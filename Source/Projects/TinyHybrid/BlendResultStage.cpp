#include "Projects/TinyHybrid/BlendResultStage.h"
#include "Graphics/Texture.h"
#include "Graphics/DXRootSignature.h"
#include "Graphics/DXComponents.h"

BlendResultStage::BlendResultStage(Texture* shadowOutput) : shadowOutput(shadowOutput)
{
	int width = DXAccess::GetWindow()->GetWindowWidth();
	int height = DXAccess::GetWindow()->GetWindowHeight();
	blendedOutput = new Texture(width, height);

	CD3DX12_DESCRIPTOR_RANGE1 sceneRenderRange[1];
	sceneRenderRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE1 shadowCaptureRange[1];
	shadowCaptureRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0);

	CD3DX12_DESCRIPTOR_RANGE1 blendBufferRange[1];
	blendBufferRange[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 1);

	CD3DX12_ROOT_PARAMETER1 rootParameters[3];
	rootParameters[0].InitAsDescriptorTable(1, &sceneRenderRange[0]); 
	rootParameters[1].InitAsDescriptorTable(1, &shadowCaptureRange[0]);
	rootParameters[2].InitAsDescriptorTable(1, &blendBufferRange[0]);

	rootSignature = new DXRootSignature(rootParameters, _countof(rootParameters));
	computePipeline = new DXComputePipeline(rootSignature, "Source/Shaders/TinyHybrid/blendResults.compute.hlsl");
}

void BlendResultStage::RecordStage(ComPtr<ID3D12GraphicsCommandList4> commandList)
{
	ComPtr<ID3D12Resource> renderTargetBuffer = DXAccess::GetWindow()->GetCurrentScreenBuffer();

	// 1) Present UV coords onto render buffer //
	//int dispatchX = window->GetWindowWidth() / 8;
	//int dispatchY = window->GetWindowHeight() / 8;
	//commandList->SetComputeRootSignature(rootSignature->GetAddress());
	//commandList->SetPipelineState(computePipeline->GetAddress());
	//
	//commandList->SetComputeRootDescriptorTable(0, DXAccess::GetWindow()->);
	//commandList->Dispatch(dispatchX, dispatchY, 1);

	// 2) Copy output from the ray tracing pipeline to the screen buffer //
	//TransitionResource(renderTargetBuffer.Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	////commandList->CopyResource(renderTargetBuffer.Get(), output);
	//TransitionResource(renderTargetBuffer.Get(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
}