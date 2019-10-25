// 상수 버퍼
cbuffer perObjectBuffer : register(b0)
{
	matrix world;
};

cbuffer perSceneBuffer : register(b1)
{
	matrix view;
	matrix projection;
};



struct vs_input
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

struct vs_output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texCoord : TEXCOORD;
};

// 정점 셰이더
vs_output main(vs_input input) // 위치값은 float4로 넘겨야함(고정값 simd)
{

	vs_output output;

	// 공간 변환
	// 월드-뷰-투영 변환
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = input.color;
	output.texCoord = input.texCoord;


	return output;
}