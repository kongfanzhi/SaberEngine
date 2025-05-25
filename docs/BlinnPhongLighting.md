# 布林冯光照模型使用指南

## 概述

我们为 SaberEngine 的 3D 渲染器实现了布林冯（Blinn-Phong）光照模型，支持方向光和点光源，提供真实的光照效果。

## 主要特性

- **方向光（Directional Light）**: 模拟太阳光等平行光源
- **点光源（Point Light）**: 模拟灯泡等点状光源，支持距离衰减
- **材质系统**: 支持环境光、漫反射和镜面反射属性
- **多光源**: 最多支持 4 个点光源同时工作
- **布林冯镜面反射**: 比传统冯氏模型更真实的高光效果

## 快速开始

### 1. 设置光照

```cpp
#include "Saber/Renderer/LightingExample.h"

// 设置默认光照
LightingExample::SetupDefaultLighting();

// 或者设置自定义场景光照
LightingExample::SetupSceneLights();
```

### 2. 渲染带光照的场景

```cpp
#include "Saber/Renderer/Renderer3D.h"
#include "Saber/Renderer/LightingExample.h"

// 在渲染循环中
PerspectiveCamera camera(45.0f, aspectRatio, 0.1f, 100.0f);
glm::mat4 viewMatrix = camera.GetViewMatrix();

// 使用光照渲染
LightingExample::RenderLitCubes(camera, viewMatrix);
```

## 详细使用

### 方向光设置

```cpp
#include "Saber/Renderer/Light.h"

DirectionalLight dirLight;
dirLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);  // 光照方向
dirLight.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);       // 环境光强度
dirLight.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);       // 漫反射强度
dirLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);      // 镜面反射强度

LightManager::SetDirectionalLight(dirLight);
```

### 点光源设置

```cpp
PointLight pointLight;
pointLight.Position = glm::vec3(2.0f, 2.0f, 2.0f);    // 光源位置
pointLight.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);  // 环境光
pointLight.Diffuse = glm::vec3(0.8f, 0.2f, 0.2f);     // 漫反射（红色光）
pointLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);    // 镜面反射

// 衰减参数
pointLight.Constant = 1.0f;      // 常数衰减
pointLight.Linear = 0.09f;       // 线性衰减
pointLight.Quadratic = 0.032f;   // 二次衰减

LightManager::AddPointLight(pointLight);
```

### 材质设置

```cpp
Material material;
material.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);   // 环境光反射率
material.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);   // 漫反射率
material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);  // 镜面反射率
material.Shininess = 64.0f;                        // 光泽度（越高越亮）

LightManager::SetMaterial(material);
```

### 自定义渲染

```cpp
// 开始光照场景
glm::vec3 cameraPos = camera.GetPosition();
Renderer3D::BeginSceneWithLighting(camera, viewMatrix, cameraPos);

// 渲染物体
Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), 
                     glm::vec3(1.0f), 
                     glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

// 结束场景
Renderer3D::EndScene();
```

## 光照模型说明

### 布林冯光照模型组成

1. **环境光（Ambient）**: 模拟间接光照，提供基础亮度
2. **漫反射（Diffuse）**: 根据表面法线和光线方向计算，遵循兰伯特定律
3. **镜面反射（Specular）**: 使用半角向量计算，比传统冯氏模型更高效

### 计算公式

```glsl
// 漫反射
float diff = max(dot(normal, lightDir), 0.0);

// 布林冯镜面反射
vec3 halfwayDir = normalize(lightDir + viewDir);
float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
```

### 点光源衰减

```glsl
float distance = length(lightPos - fragPos);
float attenuation = 1.0 / (constant + linear * distance + quadratic * distance * distance);
```

## 性能考虑

- 最多支持 4 个点光源，超出部分会被忽略
- 光照计算在片段着色器中进行，适合现代 GPU
- 使用批量渲染减少 draw call
- 法线已预计算并存储在顶点数据中

## 文件结构

```
Engine/src/Saber/Renderer/
├── Light.h                 # 光照结构体定义
├── Light.cpp              # 光照管理器实现
├── BlinnPhong3D.glsl      # 布林冯光照着色器
├── LightingExample.h      # 使用示例
└── LightingExample.cpp    # 示例实现
```

## 注意事项

1. 使用 `BeginSceneWithLighting()` 而不是 `BeginScene()` 来启用光照
2. 确保设置了光源和材质，否则物体可能显示为黑色
3. 相机位置对镜面反射效果很重要，需要正确传递
4. 法线向量已经在顶点数据中预计算，无需手动设置

## 示例效果

运行 `LightingExample::SetupSceneLights()` 和 `LightingExample::RenderLitCubes()` 可以看到：

- 中心有一个白色立方体
- 周围环绕 8 个彩色立方体
- 3 个不同颜色的点光源（红、绿、蓝）
- 一个模拟太阳光的方向光
- 地面反射光照效果

这个实现提供了一个完整的、易于使用的 3D 光照系统，适合各种 3D 渲染需求。 