#include "RootSignatures.hlsli"
#include "Basic.hlsli"

cbuffer PerObjectCb : register(b0) {
	float4x4 modelMat;
}

[RootSignature(RootSig2)]
VSOutput main(IAOutput iao) {
	VSOutput vso;
	vso.position = mul(modelMat, float4(iao.position, 1.0f));
	vso.normal = float4(iao.normal, 0.0f);
    vso.texCoord = iao.texCoord;
	return vso;
}
