#pragma once

#include "../Common.h"
#include "Material.h"
#include "Geometry.h"
#include "../PsoManager.h"
#include <typeinfo>
#include <map>

namespace Egg {
	namespace Mesh {

		GG_CLASS(Shaded)
			com_ptr<ID3D12PipelineState> pipelineState;
			D3D12_GRAPHICS_PIPELINE_STATE_DESC gpsoDesc;
			Material::P material;
			Geometry::P geometry;

		public:
			Shaded(PsoManager::A psoMan, Material::P mat, Geometry::P geom) : pipelineState{ nullptr }, gpsoDesc{}, material{ mat }, geometry{ geom } {
				ZeroMemory(&gpsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
				gpsoDesc.NumRenderTargets = 1;
				gpsoDesc.PrimitiveTopologyType = geom->GetTopologyType();
				gpsoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
				gpsoDesc.InputLayout = geom->GetInputLayout();
				mat->ApplyToDescriptor(gpsoDesc);
			
				pipelineState = psoMan->Get(gpsoDesc);
			}

			Material::P GetMaterial() { return material; }

			Egg::Mesh::Geometry::P GetGeometry() {
				return geometry;
			}

			void SetTopology(D3D_PRIMITIVE_TOPOLOGY topo) {
				geometry->SetTopology(topo);
			}

			void Draw(ID3D12GraphicsCommandList * commandList, unsigned int objectIndex = 0) {
				commandList->SetPipelineState(pipelineState.Get());
				material->ApplyToCommandList(commandList, objectIndex);
				geometry->Draw(commandList);
			}

		GG_ENDCLASS


	}
}
