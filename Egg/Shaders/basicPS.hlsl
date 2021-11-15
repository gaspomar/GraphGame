#include "RootSignatures.hlsli"
#include "basic.hlsli"

Texture2D txt : register(t0);
SamplerState sampl : register(s0);
TextureCube env : register(t1);

[RootSignature(RootSig4)]
float4 main(VSOutput vso) : SV_Target {
	float3 viewDir = normalize(cameraPos.xyz - vso.worldPos.xyz);
	float3 normal = normalize(vso.normal);

	float4 texColor = 0.3f * txt.Sample(sampl, vso.texCoord);
	float4 envReflect = 0.3f * env.Sample(sampl, reflect(-viewDir, vso.normal));
	float4 diffuse = 0.3f * txt.Sample(sampl, vso.texCoord) * saturate(dot(vso.normal, lightPos.xyz)) * lightPowerDensity;

	float4 pixelColor = texColor + envReflect + diffuse;

	return pixelColor;;
}
