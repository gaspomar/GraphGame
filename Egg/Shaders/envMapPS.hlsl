#include "RootSignatures.hlsli"
#include "basic.hlsli"

SamplerState sampl : register(s0);
TextureCube env : register(t1);



[RootSignature(RootSig4)]
float4 main(VSOutput vso) : SV_Target{
	float3 viewDir = normalize(cameraPos.xyz - vso.worldPos.xyz);

	return env.Sample(sampl, reflect(-viewDir, vso.normal));
}