#include "RootSignatures.hlsli"
#include "cbQuad.hlsli"


SamplerState sampl : register(s0);
TextureCube env : register(t1);


[RootSignature(RootSig4)]
float4 main(VSOutput vso) : SV_Target{
	return env.Sample(sampl, vso.rayDir);
}