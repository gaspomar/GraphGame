struct IAOutput {
	float3 position : POSITION;
};

struct VSOutput {
	float4 position : SV_Position;
	float4 rayDir : RAY_DIR;
};