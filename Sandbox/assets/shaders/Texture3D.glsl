#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in float a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;
out float v_Depth;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    v_TilingFactor = a_TilingFactor;

    vec4 position = u_ViewProjection * vec4(a_Position, 1.0);
    v_Depth = position.z / position.w;
    gl_Position = position;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;
in float v_Depth;

uniform sampler2D u_Textures[32];

void main()
{
    // 简化着色器逻辑，直接使用白色纹理
    vec4 texColor = texture(u_Textures[0], v_TexCoord * v_TilingFactor);
    color = texColor * v_Color;
}