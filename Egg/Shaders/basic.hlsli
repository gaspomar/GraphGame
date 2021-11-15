struct IAOutput {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	uint vertexId : SV_VertexID;
};

struct VSOutput {
	float4 position : SV_Position;
	float4 worldPos : WORLD;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// ========================================

cbuffer PerObjectCb : register(b0) {
	float4x4 modelMat;
	float4x4 modelMatInv;
}

cbuffer PerFrameCb : register(b1) {
	float4x4 viewProjMat;
	float4x4 rayDir;
	float4 cameraPos;
	float4 lightPos;
	float4 lightPowerDensity;
}
