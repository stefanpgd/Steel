#pragma once
#include "Graphics/RenderStage.h"
#include "Graphics/DXCommon.h"
#include "Framework/Mathematics.h"

class Scene;
class Texture;
class DXUploadBuffer;
class DXRayTracingPipeline;
class DXTopLevelAS;
class DXShaderBindingTable;

struct RTShadowInfo
{
	glm::vec3 sunDirection;
	float stub[61];
};

class RTShadowStage : public RenderStage
{
public:
	RTShadowStage(Scene* sceneToRender);

	void RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList) override;

private:
	void CreateShaderResources();
	void InitializePipeline();
	void InitializeShaderBindingTable();

private:
	Scene* activeScene;
	RTShadowInfo shadowInfo;

	Texture* outputBuffer;
	DXUploadBuffer* shadowInfoBuffer;

	DXTopLevelAS* TLAS;
	DXRayTracingPipeline* rayTracePipeline;
	DXShaderBindingTable* shaderTable;
};