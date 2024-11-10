#include "Projects/TinyHybrid/RTShadowsStage.h"
#include "Graphics/Texture.h"
#include "Graphics/DXUploadBuffer.h"
#include "Graphics/DXComponents.h"
#include "Graphics/DXR/DXRayTracingPipeline.h"
#include "Graphics/DXR/DXTLAS.h"
#include "Graphics/DXR/DXShaderBindingTable.h"
#include "Framework/Scene.h"

#include "Graphics/Mesh.h"
#include "Graphics/Model.h"

RTShadowStage::RTShadowStage(Scene* sceneToRender) : activeScene(sceneToRender)
{
	CreateShaderResources();

	TLAS = new DXTopLevelAS(sceneToRender);
	InitializePipeline();
	InitializeShaderBindingTable();
}

void RTShadowStage::RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList)
{
	ComPtr<ID3D12Resource> renderTargetBuffer = DXAccess::GetWindow()->GetCurrentScreenBuffer();
	ID3D12Resource* const output = outputBuffer->GetAddress();

	// 1) Prepare render buffer & Run the ray tracing pipeline // 
	TransitionResource(output, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_UNORDERED_ACCESS);

	commandList->SetPipelineState1(rayTracePipeline->GetPipelineState());
	commandList->DispatchRays(shaderTable->GetDispatchRayDescription());

	TransitionResource(output, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, D3D12_RESOURCE_STATE_COPY_SOURCE);

	// 2) Copy output from the ray tracing pipeline to the screen buffer //
	TransitionResource(renderTargetBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_DEST);
	commandList->CopyResource(renderTargetBuffer.Get(), output);
	TransitionResource(renderTargetBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void RTShadowStage::CreateShaderResources()
{
	int width = DXAccess::GetWindow()->GetWindowWidth();
	int height = DXAccess::GetWindow()->GetWindowHeight();

	outputBuffer = new Texture(width, height, DXGI_FORMAT_R8G8B8A8_UNORM);
	shadowInfoBuffer = new DXUploadBuffer(&shadowInfo, sizeof(RTShadowInfo));
}

void RTShadowStage::InitializePipeline()
{
	DXRayTracingPipelineSettings settings;
	settings.maxRayRecursionDepth = 4;

	settings.rayGenPath = L"Source/Shaders/RTShadows/RayGen.hlsl";
	settings.closestHitPath = L"Source/Shaders/RTShadows/ClosestHit.hlsl";
	settings.missPath = L"Source/Shaders/RTShadows/Miss.hlsl";

	// RayGen Root //
	CD3DX12_DESCRIPTOR_RANGE rayGenRange[1];
	rayGenRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0, 0); // Output Buffer 

	CD3DX12_ROOT_PARAMETER rayGenParameters[2];
	rayGenParameters[0].InitAsDescriptorTable(_countof(rayGenRange), &rayGenRange[0]);
	rayGenParameters[1].InitAsShaderResourceView(0, 0); // BVH

	settings.rayGenParameters = &rayGenParameters[0];
	settings.rayGenParameterCount = _countof(rayGenParameters);

	// Hit Root //
	CD3DX12_ROOT_PARAMETER hitParameters[3];
	hitParameters[0].InitAsShaderResourceView(0, 0); // Vertex buffer
	hitParameters[1].InitAsShaderResourceView(1, 0); // Index buffer
	hitParameters[2].InitAsShaderResourceView(2, 0); // TLAS Scene 

	settings.hitParameters = &hitParameters[0];
	settings.hitParameterCount = _countof(hitParameters);

	// Miss Root //
	settings.missParameters = nullptr;
	settings.missParameterCount = 0;

	settings.payLoadSize = sizeof(float) * 5; // RGB, Depth, Seed
	rayTracePipeline = new DXRayTracingPipeline(settings);
}

void RTShadowStage::InitializeShaderBindingTable()
{
	if(!shaderTable)
	{
		shaderTable = new DXShaderBindingTable(rayTracePipeline->GetPipelineProperties());
	}

	// Ray Gen Entry //
	DXDescriptorHeap* heap = DXAccess::GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	auto targetBuffer = reinterpret_cast<UINT64*>(outputBuffer->GetUAV().ptr);
	auto tlasPtr = reinterpret_cast<UINT64*>(TLAS->GetGPUVirtualAddress());
	shaderTable->AddRayGenerationProgram(L"RayGen", { targetBuffer, tlasPtr });

	// Mis Entry //
	shaderTable->AddMissProgram(L"Miss", { });

	// Hit Entries //
	const std::vector<Model*>& models = activeScene->GetModels();
	for(Model* model : models)
	{
		const std::vector<Mesh*>& meshes = model->GetMeshes();
		for(Mesh* mesh : meshes)
		{
			auto vertex = reinterpret_cast<UINT64*>(mesh->GetVertexBuffer()->GetGPUVirtualAddress());
			auto index = reinterpret_cast<UINT64*>(mesh->GetIndexBuffer()->GetGPUVirtualAddress());
			shaderTable->AddHitProgram(L"HitGroup", { vertex, index, tlasPtr });
		}
	}

	shaderTable->BuildShaderTable();
}