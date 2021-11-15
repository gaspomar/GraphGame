#pragma once

#include <Egg/SimpleApp.h>
#include <Egg/Importer.h>
#include <Egg/Math/Math.h>
#include <Egg/ConstantBuffer.hpp>
#include <Egg/Cam/FirstPerson.h>
#include <Egg/Mesh/Prefabs.h>
#include <Egg/Mesh/Multi.h>
#include <Egg/Scene/Entity.h>
#include <Egg/Scene/FixedRigidBody.h>
#include <Egg/Scene/ManagerApp.h>

#include "Particle.h"
#include <vector>

using namespace Egg::Math;

class ggl004App : public Egg::Scene::ManagerApp {
protected:
	std::vector<Particle> particles;
	Egg::Mesh::VertexStreamGeometry::P particlesGeometry;
	Egg::Mesh::Shaded::P fireBillboardSet;

public:

	virtual void PopulateCommandList() override {
		commandAllocator->Reset();
		commandList->Reset(commandAllocator.Get(), nullptr);

		commandList->RSSetViewports(1, &viewPort);
		commandList->RSSetScissorRects(1, &scissorRect);

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorHandleIncrementSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsvHeap->GetCPUDescriptorHandleForHeapStart());
		commandList->OMSetRenderTargets(1, &rHandle, FALSE, &dsvHandle);

		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		commandList->ClearRenderTargetView(rHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		__super::PopulateCommandList();
		fireBillboardSet->Draw(commandList.Get());

		commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		DX_API("Failed to close command list")
			commandList->Close();


	}

	virtual void CreateResources() override {
		__super::CreateResources();
	}

	void UploadResources() {
		DX_API("Failed to reset command allocator (UploadResources)")
			commandAllocator->Reset();
		DX_API("Failed to reset command list (UploadResources)")
			commandList->Reset(commandAllocator.Get(), nullptr);

		__super::UploadResources();

		DX_API("Failed to close command list (UploadResources)")
			commandList->Close();

		ID3D12CommandList* commandLists[] = { commandList.Get() };
		commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

		WaitForPreviousFrame();

		__super::ReleaseUploadResources();
	}

	virtual void ReleaseResources() override {
		__super::ReleaseResources();
	}

	virtual void LoadAssets() override {

		using namespace Egg::Scene;
		using namespace Egg::Mesh;

		__super::LoadAssets();

		Multi::P podMesh = ManagerApp::LoadMultiMesh("giraffe.obj", 0, "giraffe");
		

		// Create entities

		Egg::Scene::FixedRigidBody::P RigidBody1 = Egg::Scene::FixedRigidBody::Create();
		Egg::Scene::FixedRigidBody::P RigidBody2 = Egg::Scene::FixedRigidBody::Create();
		Egg::Scene::FixedRigidBody::P RigidBody3 = Egg::Scene::FixedRigidBody::Create();

		RigidBody1->Translate(Egg::Math::Float3(-0.1f, 0.2f, 0.2f));
		RigidBody2->Translate(Egg::Math::Float3(-0.3f, 0.4f, 0.1f));
		RigidBody3->Translate(Egg::Math::Float3(0.5f, -0.2f, 0.15f));

		Egg::Scene::Entity::P entity1 = Egg::Scene::Entity::Create(podMesh, RigidBody1);
		Egg::Scene::Entity::P entity2 = Egg::Scene::Entity::Create(podMesh, RigidBody2);
		Egg::Scene::Entity::P entity3 = Egg::Scene::Entity::Create(podMesh, RigidBody3);

		entities.push_back(entity1);
		entities.push_back(entity2);
		entities.push_back(entity3);

		// ------------------------------ Particles -------------------------------------

		for (int i = 0; i < 40; i++)
			particles.push_back(Particle());

		particlesGeometry =
			Egg::Mesh::VertexStreamGeometry::Create(device.Get(),
				particles.data(),
				sizeof(particles[0]) * particles.size(),
				sizeof(particles[0]));
		particlesGeometry->SetTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		D3D12_INPUT_ELEMENT_DESC particlePositionDesc;
		particlePositionDesc.AlignedByteOffset = offsetof(Particle, position);
		particlePositionDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		particlePositionDesc.InputSlot = 0;
		particlePositionDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		particlePositionDesc.InstanceDataStepRate = 0;
		particlePositionDesc.SemanticIndex = 0;
		particlePositionDesc.SemanticName = "POSITION";

		particlesGeometry->AddInputElement(particlePositionDesc);
		D3D12_INPUT_ELEMENT_DESC particleLifespanDesc;
		particleLifespanDesc.AlignedByteOffset = offsetof(Particle, lifespan);
		particleLifespanDesc.Format = DXGI_FORMAT_R32_FLOAT;
		particleLifespanDesc.InputSlot = 0;
		particleLifespanDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		particleLifespanDesc.InstanceDataStepRate = 0;
		particleLifespanDesc.SemanticIndex = 0;
		particleLifespanDesc.SemanticName = "LIFESPAN";
		particlesGeometry->AddInputElement(particleLifespanDesc);

		D3D12_INPUT_ELEMENT_DESC particleAgeDesc;
		particleAgeDesc.AlignedByteOffset = offsetof(Particle, age);
		particleAgeDesc.Format = DXGI_FORMAT_R32_FLOAT;
		particleAgeDesc.InputSlot = 0;
		particleAgeDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		particleAgeDesc.InstanceDataStepRate = 0;
		particleAgeDesc.SemanticIndex = 0;
		particleAgeDesc.SemanticName = "AGE";
		particlesGeometry->AddInputElement(particleAgeDesc);

		com_ptr<ID3DBlob> billboardVertexShader = Egg::Shader::LoadCso("Shaders/billboardVS.cso");
		com_ptr<ID3DBlob> billboardGeometryShader = Egg::Shader::LoadCso("Shaders/billboardGS.cso");
		com_ptr<ID3DBlob> firePixelShader = Egg::Shader::LoadCso("Shaders/firePS.cso");
		com_ptr<ID3D12RootSignature> billboardRootSig = Egg::Shader::LoadRootSignature(device.Get(), billboardVertexShader.Get());

		Egg::Mesh::Material::P fireMaterial = Egg::Mesh::Material::Create();
		fireMaterial->SetRootSignature(billboardRootSig);
		fireMaterial->SetVertexShader(billboardVertexShader);
		fireMaterial->SetGeometryShader(billboardGeometryShader);
		fireMaterial->SetPixelShader(firePixelShader);
		fireMaterial->SetDepthStencilState(CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT));
		fireMaterial->SetDSVFormat(DXGI_FORMAT_D32_FLOAT);
		//fireMaterial->SetConstantBuffer(perObjectCb);
		fireMaterial->SetConstantBuffer(perFrameCb);


		fireBillboardSet = Egg::Mesh::Shaded::Create(psoManager, fireMaterial, particlesGeometry);

		// ----------------------------------------------------------------------------------

		UploadResources();
	}

	virtual void Update(float dt, float T) override
	{
		perFrameCb->billboardSize = Float4(0.1, 0.1, 0, 0);
		ManagerApp::Update(dt, T);
	}
};


