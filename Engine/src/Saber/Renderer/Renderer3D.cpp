#include "Renderer3D.h"
#include "Saber/pch.h"

#include "Camera.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"
#include <filesystem>
#include <glad/glad.h>

namespace Saber
{
struct CubeVertex
{
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
};

struct Renderer3DData
{
    static const uint32_t MaxCubes = 10000;
    static const uint32_t MaxVertices = MaxCubes * 8; // 立方体有8个顶点
    static const uint32_t MaxIndices = MaxCubes * 36; // 立方体有6个面, 每个面2个三角形, 每个三角形3个索引
    static const uint32_t MaxTextureSlots = 32;       // TODO: RenderCaps

    Ref<VertexArray> CubeVertexArray;
    Ref<VertexBuffer> CubeVertexBuffer;
    Ref<Shader> TextureShader;
    Ref<Texture2D> WhiteTexture;

    uint32_t CubeIndexCount = 0;
    CubeVertex *CubeVertexBufferBase = nullptr;
    CubeVertex *CubeVertexBufferPtr = nullptr;

    std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
    uint32_t TextureSlotIndex = 1; // 0 = white texture

    // 立方体的8个顶点位置
    glm::vec3 CubeVertexPositions[8];

    Renderer3D::Statistics Stats;
};

static Renderer3DData s_Data;

void Renderer3D::Init()
{
    // 启用深度测试，禁用背面剔除
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // 设置深度测试函数为LESS（默认值）
    glDepthFunc(GL_LESS);

    // 确保深度写入已启用
    glDepthMask(GL_TRUE);

    // 确保深度范围正确设置
    glDepthRange(0.0f, 1.0f);

    s_Data.CubeVertexArray = VertexArray::Create();

    s_Data.CubeVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CubeVertex));
    s_Data.CubeVertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"},
                                        {ShaderDataType::Float4, "a_Color"},
                                        {ShaderDataType::Float2, "a_TexCoord"},
                                        {ShaderDataType::Float, "a_TexIndex"},
                                        {ShaderDataType::Float, "a_TilingFactor"}});
    s_Data.CubeVertexArray->AddVertexBuffer(s_Data.CubeVertexBuffer);

    s_Data.CubeVertexBufferBase = new CubeVertex[s_Data.MaxVertices];

    // 创建立方体索引
    uint32_t *cubeIndices = new uint32_t[s_Data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_Data.MaxIndices; i += 36)
    {
        // 前面
        cubeIndices[i + 0] = offset + 0;
        cubeIndices[i + 1] = offset + 1;
        cubeIndices[i + 2] = offset + 2;
        cubeIndices[i + 3] = offset + 2;
        cubeIndices[i + 4] = offset + 3;
        cubeIndices[i + 5] = offset + 0;

        // 后面
        cubeIndices[i + 6] = offset + 4;
        cubeIndices[i + 7] = offset + 5;
        cubeIndices[i + 8] = offset + 6;
        cubeIndices[i + 9] = offset + 6;
        cubeIndices[i + 10] = offset + 7;
        cubeIndices[i + 11] = offset + 4;

        // 顶面
        cubeIndices[i + 12] = offset + 0;
        cubeIndices[i + 13] = offset + 4;
        cubeIndices[i + 14] = offset + 7;
        cubeIndices[i + 15] = offset + 7;
        cubeIndices[i + 16] = offset + 3;
        cubeIndices[i + 17] = offset + 0;

        // 底面
        cubeIndices[i + 18] = offset + 1;
        cubeIndices[i + 19] = offset + 5;
        cubeIndices[i + 20] = offset + 6;
        cubeIndices[i + 21] = offset + 6;
        cubeIndices[i + 22] = offset + 2;
        cubeIndices[i + 23] = offset + 1;

        // 左面
        cubeIndices[i + 24] = offset + 0;
        cubeIndices[i + 25] = offset + 1;
        cubeIndices[i + 26] = offset + 5;
        cubeIndices[i + 27] = offset + 5;
        cubeIndices[i + 28] = offset + 4;
        cubeIndices[i + 29] = offset + 0;

        // 右面
        cubeIndices[i + 30] = offset + 3;
        cubeIndices[i + 31] = offset + 2;
        cubeIndices[i + 32] = offset + 6;
        cubeIndices[i + 33] = offset + 6;
        cubeIndices[i + 34] = offset + 7;
        cubeIndices[i + 35] = offset + 3;

        offset += 8;
    }

    Ref<IndexBuffer> cubeIB = IndexBuffer::Create(cubeIndices, s_Data.MaxIndices);
    s_Data.CubeVertexArray->SetIndexBuffer(cubeIB);
    delete[] cubeIndices;

    s_Data.WhiteTexture = Texture2D::Create(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[s_Data.MaxTextureSlots];
    for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
        samplers[i] = i;

    // 使用相对路径直接加载着色器
    // std::string shaderPath = (std::filesystem::current_path() / "assets" / "shaders" / "Texture3D.glsl").string();
    s_Data.TextureShader = Shader::Create("assets/shaders/Texture3D.glsl");
    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

    // 设置所有纹理槽为白色纹理
    s_Data.TextureSlots[0] = s_Data.WhiteTexture;

    // 初始化立方体顶点位置
    // 前面四个顶点
    s_Data.CubeVertexPositions[0] = {-0.5f, -0.5f, 0.5f}; // 左下前
    s_Data.CubeVertexPositions[1] = {0.5f, -0.5f, 0.5f};  // 右下前
    s_Data.CubeVertexPositions[2] = {0.5f, 0.5f, 0.5f};   // 右上前
    s_Data.CubeVertexPositions[3] = {-0.5f, 0.5f, 0.5f};  // 左上前

    // 后面四个顶点
    s_Data.CubeVertexPositions[4] = {-0.5f, -0.5f, -0.5f}; // 左下后
    s_Data.CubeVertexPositions[5] = {0.5f, -0.5f, -0.5f};  // 右下后
    s_Data.CubeVertexPositions[6] = {0.5f, 0.5f, -0.5f};   // 右上后
    s_Data.CubeVertexPositions[7] = {-0.5f, 0.5f, -0.5f};  // 左上后
}

void Renderer3D::Shutdown() { delete[] s_Data.CubeVertexBufferBase; }

void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 &transform)
{
    // 创建视图投影矩阵 - 将相机的投影矩阵与传入的变换矩阵相乘
    // transform是相机的视图矩阵
    glm::mat4 viewProj = camera.GetProjection() * transform;

    // 确保OpenGL深度测试正确配置
    if (!glIsEnabled(GL_DEPTH_TEST))
    {
        glEnable(GL_DEPTH_TEST);
    }

    // 设置深度函数为LESS（默认）
    glDepthFunc(GL_LESS);

    // 确保深度掩码已启用（默认允许深度写入）
    glDepthMask(GL_TRUE);

    s_Data.TextureShader->Bind();
    s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

    s_Data.CubeIndexCount = 0;
    s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}

void Renderer3D::EndScene()
{
    uint32_t dataSize = (uint8_t *)s_Data.CubeVertexBufferPtr - (uint8_t *)s_Data.CubeVertexBufferBase;
    s_Data.CubeVertexBuffer->SetData(s_Data.CubeVertexBufferBase, dataSize);

    Flush();
}

void Renderer3D::Flush()
{
    // 绑定纹理
    for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
        s_Data.TextureSlots[i]->Bind(i);

    RenderCommand::DrawIndexed(s_Data.CubeVertexArray, s_Data.CubeIndexCount);
    s_Data.Stats.DrawCalls++;
}

void Renderer3D::DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color)
{
    if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float textureIndex = 0.0f; // 使用白色纹理
    const float tilingFactor = 1.0f;

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);

    // 设置8个顶点的数据
    for (int i = 0; i < 8; i++)
    {
        s_Data.CubeVertexBufferPtr->Position = transform * glm::vec4(s_Data.CubeVertexPositions[i], 1.0f);
        s_Data.CubeVertexBufferPtr->Color = color;

        // 纹理坐标根据顶点位置计算
        float u = (i == 1 || i == 2 || i == 5 || i == 6) ? 1.0f : 0.0f;
        float v = (i == 2 || i == 3 || i == 6 || i == 7) ? 1.0f : 0.0f;
        s_Data.CubeVertexBufferPtr->TexCoord = {u, v};

        s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.CubeVertexBufferPtr++;
    }

    s_Data.CubeIndexCount += 36;
    s_Data.Stats.CubeCount++;
}

void Renderer3D::DrawCube(const glm::vec3 &position, const glm::vec3 &size, const Ref<Texture2D> &texture,
                          float tilingFactor, const glm::vec4 &tintColor)
{
    if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
        if (*s_Data.TextureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);

    // 设置8个顶点的数据
    for (int i = 0; i < 8; i++)
    {
        s_Data.CubeVertexBufferPtr->Position = transform * glm::vec4(s_Data.CubeVertexPositions[i], 1.0f);
        s_Data.CubeVertexBufferPtr->Color = tintColor;

        // 纹理坐标根据顶点位置计算
        float u = (i == 1 || i == 2 || i == 5 || i == 6) ? 1.0f : 0.0f;
        float v = (i == 2 || i == 3 || i == 6 || i == 7) ? 1.0f : 0.0f;
        s_Data.CubeVertexBufferPtr->TexCoord = {u, v};

        s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.CubeVertexBufferPtr++;
    }

    s_Data.CubeIndexCount += 36;
    s_Data.Stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &rotation,
                                 const glm::vec4 &color)
{
    if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    const float textureIndex = 0.0f; // 使用白色纹理
    const float tilingFactor = 1.0f;

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

    // 按照X、Y、Z轴顺序应用旋转
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1.0f, 0.0f, 0.0f});
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0.0f, 1.0f, 0.0f});
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0.0f, 0.0f, 1.0f});

    transform = transform * glm::scale(glm::mat4(1.0f), size);

    // 设置8个顶点的数据
    for (int i = 0; i < 8; i++)
    {
        s_Data.CubeVertexBufferPtr->Position = transform * glm::vec4(s_Data.CubeVertexPositions[i], 1.0f);
        s_Data.CubeVertexBufferPtr->Color = color;

        // 纹理坐标根据顶点位置计算
        float u = (i == 1 || i == 2 || i == 5 || i == 6) ? 1.0f : 0.0f;
        float v = (i == 2 || i == 3 || i == 6 || i == 7) ? 1.0f : 0.0f;
        s_Data.CubeVertexBufferPtr->TexCoord = {u, v};

        s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.CubeVertexBufferPtr++;
    }

    s_Data.CubeIndexCount += 36;
    s_Data.Stats.CubeCount++;
}

void Renderer3D::DrawRotatedCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &rotation,
                                 const Ref<Texture2D> &texture, float tilingFactor, const glm::vec4 &tintColor)
{
    if (s_Data.CubeIndexCount >= Renderer3DData::MaxIndices)
        FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
    {
        if (*s_Data.TextureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        textureIndex = (float)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
        s_Data.TextureSlotIndex++;
    }

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

    // 按照X、Y、Z轴顺序应用旋转
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1.0f, 0.0f, 0.0f});
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0.0f, 1.0f, 0.0f});
    transform = transform * glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0.0f, 0.0f, 1.0f});

    transform = transform * glm::scale(glm::mat4(1.0f), size);

    // 设置8个顶点的数据
    for (int i = 0; i < 8; i++)
    {
        s_Data.CubeVertexBufferPtr->Position = transform * glm::vec4(s_Data.CubeVertexPositions[i], 1.0f);
        s_Data.CubeVertexBufferPtr->Color = tintColor;

        // 纹理坐标根据顶点位置计算
        float u = (i == 1 || i == 2 || i == 5 || i == 6) ? 1.0f : 0.0f;
        float v = (i == 2 || i == 3 || i == 6 || i == 7) ? 1.0f : 0.0f;
        s_Data.CubeVertexBufferPtr->TexCoord = {u, v};

        s_Data.CubeVertexBufferPtr->TexIndex = textureIndex;
        s_Data.CubeVertexBufferPtr->TilingFactor = tilingFactor;
        s_Data.CubeVertexBufferPtr++;
    }

    s_Data.CubeIndexCount += 36;
    s_Data.Stats.CubeCount++;
}

void Renderer3D::ResetStats() { memset(&s_Data.Stats, 0, sizeof(Statistics)); }

Renderer3D::Statistics Renderer3D::GetStats() { return s_Data.Stats; }

void Renderer3D::FlushAndReset()
{
    EndScene();

    s_Data.CubeIndexCount = 0;
    s_Data.CubeVertexBufferPtr = s_Data.CubeVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
}
} // namespace Saber