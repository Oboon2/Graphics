// 상수 버퍼
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
    matrix viewProjection;
    float3 worldLightPosition;
    float3 worldCameraPosition;
};



struct vs_input
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct vs_output
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float3 lightDir : TEXCOORD1;
    //float3 viewDir : TEXCOORD2;
};

// 정점 셰이더
//float4 main(float4 pos : POSITION, float4 color : COLOR) : SV_POSITION
vs_output main(vs_input input) // 위치값은 float4로 넘겨야함(고정값 simd)
{

	vs_output output;

	// 공간 변환
	// 월드-뷰-투영 변환
	output.position = mul(input.position, world);
// 라이트 방향 벡터.
    output.lightDir = normalize(worldLightPosition - output.position.xyz);

    output.position = mul(output.position, viewProjection);
    output.texCoord = input.texCoord;

    return output;
}