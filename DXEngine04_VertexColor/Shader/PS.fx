struct ps_input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 픽셀 셰이더
float4 main(ps_input input) : SV_TARGET // 위치값은 float4로 넘겨야함(고정값 simd)
{
	
	return input.color;
}