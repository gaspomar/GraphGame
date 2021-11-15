#include "Billboard.hlsli"

[RootSignature(BillboardRootSig)]
float4 main(GSOutput input) : SV_Target
{
   return input.tex.xyyy;
}
