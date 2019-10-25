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

// ���� ���̴�
vs_output main(vs_input input) // ��ġ���� float4�� �Ѱܾ���(������ simd)
{

	vs_output output;
	output.position = input.position;
	output.color = input.color;


	return output;
}