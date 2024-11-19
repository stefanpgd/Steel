#include "Graphics/DXBuffer.h"
#include "Graphics/DXUtilities.h"
#include "Graphics/DXDescriptorHeap.h"
#include <string>

// Things to figure out and note down
// - The differences in CBV and SRV descriptions, why is one simpler than the other
// - The difference on the HLSL side with CBV and SRV
// - Learn about rowPitch and the such, it's something I should really understand

// As far as I'm aware, either your buffer is a CBV, or treated as an SRV with UAV functionality

DXBuffer::DXBuffer(const void* data, unsigned int numberOfElements, unsigned int elementSize)
	: numberOfElements(numberOfElements), elementSize(elementSize)
{
	bufferSize = numberOfElements * elementSize;

	AllocateResource();
	UploadData(data);
	CreateDescriptor();
}

DXBuffer::DXBuffer(DXBufferProperties properties, const void* data, unsigned int numberOfElements, unsigned int elementSize)
	: bufferProperties(properties), numberOfElements(numberOfElements), elementSize(elementSize)
{
	bufferSize = numberOfElements * elementSize;

	AllocateResource();
	UploadData(data);
	CreateDescriptor();
}

void DXBuffer::UpdateData(void* data)
{

}

void DXBuffer::Resize()
{

}

void DXBuffer::AllocateResource()
{
	ComPtr<ID3D12Device5> device = DXAccess::GetDevice();
	CD3DX12_HEAP_PROPERTIES gpuHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	if(bufferProperties.isCPUAccesible)
	{
		gpuHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	}

	D3D12_RESOURCE_DESC bufferDescription = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// Unordered access needs to be marked in the resource, not in the descriptor
	if(bufferProperties.isUnorderedAccess)
	{
		bufferDescription.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	// Resources that get created on the UPLOAD heap should use the `D3D12_RESOURCE_STATE_GENERIC_READ`.
	// This state also matches with COPY_SOURCE and PIXEL_RESOURCE. Meaning that usually we shouldn't have to change
	// the resource's state. 
	resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
	ThrowIfFailed(device->CreateCommittedResource(&gpuHeap, D3D12_HEAP_FLAG_NONE, &bufferDescription,
		resourceState, nullptr, IID_PPV_ARGS(&buffer)));

	std::wstring resourceName = std::wstring(bufferProperties.name.begin(), bufferProperties.name.end());
	buffer->SetName(resourceName.c_str());
}

void DXBuffer::UploadData(const void* data)
{
	// Whether or not it's a UPLOAD or DEFAULT allocated resources, adjust the matter of uploading data.
}

void DXBuffer::CreateDescriptor()
{
}

#pragma region Getters
unsigned int DXBuffer::GetBufferSize()
{
	return 0;
}

ComPtr<ID3D12Resource> DXBuffer::GetResource()
{
	return ComPtr<ID3D12Resource>();
}

D3D12_GPU_VIRTUAL_ADDRESS DXBuffer::GetGPUVirtualAddress()
{
	return D3D12_GPU_VIRTUAL_ADDRESS();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXBuffer::GetCBV()
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXBuffer::GetSRV()
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE();
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DXBuffer::GetUAV()
{
	return CD3DX12_GPU_DESCRIPTOR_HANDLE();
}
#pragma endregion