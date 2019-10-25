struct vs_input
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct vs_output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 정점 셰이더
vs_output main(vs_input input) // 위치값은 float4로 넘겨야함(고정값 simd)
{

	vs_output output;
	output.position = input.position;
	output.color = input.color;


	return output;
}