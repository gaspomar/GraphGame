#include "RootSignatures.hlsli"
#include "basic.hlsli"


[RootSignature(RootSig4)]
VSOutput main(IAOutput iao) {
	VSOutput vso;
	vso.worldPos = mul(modelMat, float4(iao.position, 1.0f));
	vso.position = mul(viewProjMat, vso.worldPos);
	vso.normal = float4(iao.normal, 0.0f);
	vso.texCoord = iao.texCoord;
	return vso;
}
