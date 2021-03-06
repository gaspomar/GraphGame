#pragma once

#include "Common.h"

namespace Egg {

	class TextureCube {
		com_ptr<ID3D12Resource> resource;
		com_ptr<ID3D12Resource> uploadResource;
		D3D12_RESOURCE_DESC resourceDesc;
	public:
		TextureCube();

		// copy-and-swap idiom (c++11)
		TextureCube & operator=(TextureCube t);

		TextureCube(com_ptr<ID3D12Resource> && resource, com_ptr<ID3D12Resource> && uploadResource, D3D12_RESOURCE_DESC resourceDesc);

		void UploadResource(ID3D12GraphicsCommandList * commandList);

		void ReleaseUploadResources();

		void CreateSRV(ID3D12Device * device, ID3D12DescriptorHeap * owner, unsigned int index) {
			static unsigned int incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

			CD3DX12_CPU_DESCRIPTOR_HANDLE handle{ owner->GetCPUDescriptorHandleForHeapStart(), (int)(index * incrementSize) };

			D3D12_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(D3D12_SHADER_RESOURCE_VIEW_DESC));
			srvd.Format = resourceDesc.Format;
			srvd.TextureCube.MipLevels = resourceDesc.MipLevels;
			srvd.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			srvd.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;

			device->CreateShaderResourceView(resource.Get(), &srvd, handle);
		}

		ID3D12Resource * operator->() {
			return resource.Get();
		}
	};

}
