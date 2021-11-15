#include "RootSignatures.hlsli"
#include "cbQuad.hlsli"

cbuffer PerObjectCb : register(b0) {
	float4x4 modelMat;
}

cbuffer PerFrameCb : register(b1) {
	float4x4 trafoMat;
	float4x4 rayDirMat;
}


[RootSignature(RootSig4)]
VSOutput main(IAOutput iao) {
	VSOutput vso;
	vso.position = float4(iao.position[0], iao.position[1], 0.9999999f, 1.0f);
	vso.rayDir = mul(rayDirMat, vso.position);
	return vso;
}
