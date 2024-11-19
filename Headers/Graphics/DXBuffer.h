#pragma once
#include "DXCommon.h"

// Notes to self:
// - The GPU heap where the resources gets uploaded determines whether the resource is CPU accessible or not
// meaning that we can or can't map directly to it. 

/// <summary>
/// Functionality description of a DXBuffer.
/// based on the settings enabled/disabled, the functionality of the buffer changes.
/// 
/// By default, DXBuffers are treated as Constant Buffers without any extra functionality
/// </summary>
struct DXBufferProperties
{
	std::string name = "";
	bool isConstantBuffer = true;
	bool isUnorderedAccess = false;
	bool isStructuredBuffer = false;
	bool isCPUAccesible = false;
};

/// <summary>
/// A generic buffer class that supports Constant Buffers, Shader Resources and Unordered Access.
/// Should be used whenever large groups of data are required on the GPU, but only buffers that are 
/// technically linear one-dimensional arrays. Such as a material buffer, or a static transform buffer.
/// 
/// So this shouldn't be used for things like 2D textures
/// </summary>
class DXBuffer
{
public:
	DXBuffer(const void* data, unsigned int numberOfElements, unsigned int elementSize);
	DXBuffer(DXBufferProperties properties, const void* data, unsigned int numberOfElements, unsigned int elementSize);

	void UpdateData(void* data);
	void Resize();

	unsigned int GetBufferSize();

	ComPtr<ID3D12Resource> GetResource();
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetCBV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetSRV();
	CD3DX12_GPU_DESCRIPTOR_HANDLE GetUAV();

private:
	void AllocateResource();
	void UploadData(const void* data);
	void CreateDescriptor();

private:
	// Resource info //
	ComPtr<ID3D12Resource> buffer;
	DXBufferProperties bufferProperties;
	D3D12_RESOURCE_STATES resourceState;

	unsigned int numberOfElements;
	unsigned int elementSize; // expressed in bytes 
	unsigned int bufferSize;

	// Descriptor info //
	unsigned int cbvIndex;
	unsigned int srvIndex;
	unsigned int uavIndex;
};