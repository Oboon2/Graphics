struct ps_input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// �ȼ� ���̴�
float4 main(ps_input input) : SV_TARGET // ��ġ���� float4�� �Ѱܾ���(������ simd)
{
	
	return input.color;
}