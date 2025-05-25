#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(location = 5) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_NormalMatrix;
uniform vec3 u_ViewPos;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;
out vec3 v_FragPos;
out vec3 v_Normal;
out vec3 v_ViewPos;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;
    v_ViewPos = u_ViewPos;

    // 计算世界空间位置
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_FragPos = worldPos.xyz;

    // 变换法线到世界空间
    v_Normal = normalize(mat3(u_NormalMatrix) * a_Normal);

    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in vec3 v_FragPos;
in vec3 v_Normal;
in vec3 v_ViewPos;

uniform sampler2D u_Textures[32];

// 光照参数
struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLights[4];
uniform int u_NumPointLights;

// 材质参数
uniform float u_Shininess;
uniform vec3 u_MaterialAmbient;
uniform vec3 u_MaterialDiffuse;
uniform vec3 u_MaterialSpecular;

// 计算方向光
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 materialColor)
{
    vec3 lightDir = normalize(-light.direction);

    // 环境光
    vec3 ambient = light.ambient * u_MaterialAmbient * materialColor;

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * u_MaterialDiffuse * materialColor;

    // 镜面反射 (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = light.specular * spec * u_MaterialSpecular;

    return ambient + diffuse + specular;
}

// 计算点光源
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialColor)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 环境光
    vec3 ambient = light.ambient * u_MaterialAmbient * materialColor;

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * u_MaterialDiffuse * materialColor;

    // 镜面反射 (Blinn-Phong)
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    vec3 specular = light.specular * spec * u_MaterialSpecular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main()
{
    // 获取纹理颜色
    vec4 texColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor);
    vec3 materialColor = texColor.rgb * v_Color.rgb;

    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(v_ViewPos - v_FragPos);

    // 计算方向光
    vec3 result = CalcDirLight(u_DirLight, norm, viewDir, materialColor);

    // 计算点光源
    for (int i = 0; i < u_NumPointLights && i < 4; i++)
    {
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir, materialColor);
    }

    color = vec4(result, texColor.a * v_Color.a);
}