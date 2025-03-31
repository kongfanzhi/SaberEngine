#include "Saber/Renderer/PerspectiveCamera.h"
#include "Saber/Renderer/RenderCommand.h"
#include "Saber/Renderer/Renderer3D.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Saber;

// ʾ�����룺չʾ���ʹ��Renderer3D
void Renderer3DExample()
{
    // ��ʼ����Ⱦ��
    Renderer3D::Init();

    // ����͸�������
    PerspectiveCamera camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // ����������任����λ�ú���ת��
    glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    // ��������
    Ref<Texture2D> texture = Texture2D::Create("assets/textures/example.png");

    // ��Ⱦѭ���еĴ���
    {
        // �����Ļ
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        // ��ʼ������Ⱦ
        Renderer3D::BeginScene(camera, cameraTransform);

        // ����һЩ������

        // 1. ʹ����ɫ����������
        Renderer3D::DrawCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}); // ��ɫ������

        // 2. ʹ���������������
        Renderer3D::DrawCube({2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, texture);

        // 3. ������ת�������壨��ɫ��
        Renderer3D::DrawRotatedCube({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {45.0f, 45.0f, 0.0f},
                                    {0.0f, 1.0f, 0.0f, 1.0f}); // ��ɫ��ת������

        // 4. ������ת�������壨����
        Renderer3D::DrawRotatedCube({0.0f, 2.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 45.0f, 45.0f}, texture, 2.0f);

        // ����������Ⱦ
        Renderer3D::EndScene();
    }

    // ��ȡͳ����Ϣ
    Renderer3D::Statistics stats = Renderer3D::GetStats();

    // ���ͳ����Ϣ
    printf("Renderer3D Stats:\n");
    printf("  Draw Calls: %d\n", stats.DrawCalls);
    printf("  Cube Count: %d\n", stats.CubeCount);

    // �ر���Ⱦ��
    Renderer3D::Shutdown();
}