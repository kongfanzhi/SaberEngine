# ���ַ����ģ��ʹ��ָ��

## ����

����Ϊ SaberEngine �� 3D ��Ⱦ��ʵ���˲��ַ루Blinn-Phong������ģ�ͣ�֧�ַ����͵��Դ���ṩ��ʵ�Ĺ���Ч����

## ��Ҫ����

- **����⣨Directional Light��**: ģ��̫�����ƽ�й�Դ
- **���Դ��Point Light��**: ģ����ݵȵ�״��Դ��֧�־���˥��
- **����ϵͳ**: ֧�ֻ����⡢������;��淴������
- **���Դ**: ���֧�� 4 �����Դͬʱ����
- **���ַ뾵�淴��**: �ȴ�ͳ����ģ�͸���ʵ�ĸ߹�Ч��

## ���ٿ�ʼ

### 1. ���ù���

```cpp
#include "Saber/Renderer/LightingExample.h"

// ����Ĭ�Ϲ���
LightingExample::SetupDefaultLighting();

// ���������Զ��峡������
LightingExample::SetupSceneLights();
```

### 2. ��Ⱦ�����յĳ���

```cpp
#include "Saber/Renderer/Renderer3D.h"
#include "Saber/Renderer/LightingExample.h"

// ����Ⱦѭ����
PerspectiveCamera camera(45.0f, aspectRatio, 0.1f, 100.0f);
glm::mat4 viewMatrix = camera.GetViewMatrix();

// ʹ�ù�����Ⱦ
LightingExample::RenderLitCubes(camera, viewMatrix);
```

## ��ϸʹ��

### ���������

```cpp
#include "Saber/Renderer/Light.h"

DirectionalLight dirLight;
dirLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);  // ���շ���
dirLight.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);       // ������ǿ��
dirLight.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);       // ������ǿ��
dirLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);      // ���淴��ǿ��

LightManager::SetDirectionalLight(dirLight);
```

### ���Դ����

```cpp
PointLight pointLight;
pointLight.Position = glm::vec3(2.0f, 2.0f, 2.0f);    // ��Դλ��
pointLight.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);  // ������
pointLight.Diffuse = glm::vec3(0.8f, 0.2f, 0.2f);     // �����䣨��ɫ�⣩
pointLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);    // ���淴��

// ˥������
pointLight.Constant = 1.0f;      // ����˥��
pointLight.Linear = 0.09f;       // ����˥��
pointLight.Quadratic = 0.032f;   // ����˥��

LightManager::AddPointLight(pointLight);
```

### ��������

```cpp
Material material;
material.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);   // �����ⷴ����
material.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);   // ��������
material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);  // ���淴����
material.Shininess = 64.0f;                        // ����ȣ�Խ��Խ����

LightManager::SetMaterial(material);
```

### �Զ�����Ⱦ

```cpp
// ��ʼ���ճ���
glm::vec3 cameraPos = camera.GetPosition();
Renderer3D::BeginSceneWithLighting(camera, viewMatrix, cameraPos);

// ��Ⱦ����
Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), 
                     glm::vec3(1.0f), 
                     glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

// ��������
Renderer3D::EndScene();
```

## ����ģ��˵��

### ���ַ����ģ�����

1. **�����⣨Ambient��**: ģ���ӹ��գ��ṩ��������
2. **�����䣨Diffuse��**: ���ݱ��淨�ߺ͹��߷�����㣬��ѭ�����ض���
3. **���淴�䣨Specular��**: ʹ�ð���������㣬�ȴ�ͳ����ģ�͸���Ч

### ���㹫ʽ

```glsl
// ������
float diff = max(dot(normal, lightDir), 0.0);

// ���ַ뾵�淴��
vec3 halfwayDir = normalize(lightDir + viewDir);
float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
```

### ���Դ˥��

```glsl
float distance = length(lightPos - fragPos);
float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
```

## ���ܿ���

- ���֧�� 4 �����Դ���������ֻᱻ����
- ���ռ�����Ƭ����ɫ���н��У��ʺ��ִ� GPU
- ʹ��������Ⱦ���� draw call
- ������Ԥ���㲢�洢�ڶ���������

## �ļ��ṹ

```
Engine/src/Saber/Renderer/
������ Light.h                 # ���սṹ�嶨��
������ Light.cpp              # ���չ�����ʵ��
������ BlinnPhong3D.glsl      # ���ַ������ɫ��
������ LightingExample.h      # ʹ��ʾ��
������ LightingExample.cpp    # ʾ��ʵ��
```

## ע������

1. ʹ�� `BeginSceneWithLighting()` ������ `BeginScene()` �����ù���
2. ȷ�������˹�Դ�Ͳ��ʣ��������������ʾΪ��ɫ
3. ���λ�öԾ��淴��Ч������Ҫ����Ҫ��ȷ����
4. ���������Ѿ��ڶ���������Ԥ���㣬�����ֶ�����

## ʾ��Ч��

���� `LightingExample::SetupSceneLights()` �� `LightingExample::RenderLitCubes()` ���Կ�����

- ������һ����ɫ������
- ��Χ���� 8 ����ɫ������
- 3 ����ͬ��ɫ�ĵ��Դ���졢�̡�����
- һ��ģ��̫����ķ����
- ���淴�����Ч��

���ʵ���ṩ��һ�������ġ�����ʹ�õ� 3D ����ϵͳ���ʺϸ��� 3D ��Ⱦ���� 