#include "RootSignatures.hlsli"
#include "basic.hlsli"

Texture2D txt : register(t0);
SamplerState sampl : register(s0);

[RootSignature(RootSig4)]
float4 main(VSOutput vso) : SV_Target{
    float3 normal = normalize(vso.normal);

    return txt.Sample(sampl, vso.texCoord) * saturate(dot(vso.normal, lightPos.xyz)) * lightPowerDensity;
}