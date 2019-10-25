struct ps_input
{
    float4 position : SV_POSITION;
	//float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};
// �ؽ�ó / ���÷� ������Ʈ
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�
float4 main(ps_input input) : SV_TARGET // ��ġ���� float4�� �Ѱܾ���(������ simd)
{
    float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
	
	//return input.color;
    return textureColor;
}