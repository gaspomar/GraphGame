#pragma once

#include <Egg/Common.h>
#include <Egg/Mesh/Shaded.h>
#include <Egg/Mesh/Prefabs.h>
#include <Egg/ConstantBuffer.hpp>
#include "ConstantBufferTypes.h"
#include <Egg/Math/Math.h>

#include <Egg/SimpleApp.h>

GG_SUBCLASS(ggl002App, Egg::SimpleApp)
	Egg::Mesh::Shaded::P shadedMesh;

	Egg::ConstantBuffer<PerObjectCb> cb;

public:
	virtual void PopulateCommandList() {
		commandAllocator->Reset();
		commandList->Reset(commandAllocator.Get(), nullptr);

		commandList->RSSetViewports(1, &viewPort);
		commandList->RSSetScissorRects(1, &scissorRect);

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorHandleIncrementSize);
		commandList->OMSetRenderTargets(1, &rHandle, FALSE, nullptr);

		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		commandList->ClearRenderTargetView(rHandle, clearColor, 0, nullptr);

		shadedMesh->Draw(commandList.Get());

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		DX_API("Failed to close command list")
			commandList->Close();
	}

	virtual void CreateResources() override {
		__super::CreateResources();
		
		cb.CreateResources(device.Get());

	}

	virtual void LoadAssets() override {
		com_ptr<ID3DBlob> vertexShader = Egg::Shader::LoadCso("Shaders/basicVS.cso");
		com_ptr<ID3DBlob> pixelShader = Egg::Shader::LoadCso("Shaders/basicPS.cso");
		com_ptr<ID3D12RootSignature> rootSig = Egg::Shader::LoadRootSignature(device.Get(), vertexShader.Get());

		Egg::Mesh::Material::P material = Egg::Mesh::Material::Create();
		material->SetRootSignature(rootSig);
		material->SetVertexShader(vertexShader);
		material->SetPixelShader(pixelShader);

		material->SetConstantBuffer(cb, 0, "PerObjectCb");

		Egg::Mesh::Geometry::P geometry = Egg::Mesh::Prefabs::UnitBox(device.Get());

		shadedMesh = Egg::Mesh::Shaded::Create(psoManager, material, geometry);

	}

	void Update(float dt, float T) override {
		using namespace Egg::Math;
		cb->modelTransform = Float4x4::Translation(Float3(0, T, T));

		cb.Upload();

	}

GG_ENDCLASS


