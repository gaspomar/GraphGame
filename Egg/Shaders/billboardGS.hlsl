#include "Billboard.hlsli"

[RootSignature(BillboardRootSig)][maxvertexcount(4)]
void main(
    point VSOutput input[1],
    inout TriangleStream<GSOutput> stream) {
    float4 hndcPos = mul(viewProjMat, mul(modelMat, float4(input[0].pos, 1)));

    //float4 hndcPos = float4(0.0f, 0.0f, 0.5f, 1.0f);

    GSOutput output;
    output.pos = hndcPos;
    output.pos.x += billboardSize.x;
    output.pos.y += billboardSize.y;
    output.tex = float2(1, 0);
    stream.Append(output);

    output.pos = hndcPos;
    output.pos.x += billboardSize.x;
    output.pos.y -= billboardSize.y;
    output.tex = float2(1, 0);
    stream.Append(output);

    output.pos = hndcPos;
    output.pos.x -= billboardSize.x;
    output.pos.y += billboardSize.y;
    output.tex = float2(1, 0);
    stream.Append(output);

    output.pos = hndcPos;
    output.pos.x -= billboardSize.x;
    output.pos.y -= billboardSize.y;
    output.tex = float2(1, 0);
    stream.Append(output);

}
