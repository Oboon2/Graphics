struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};
// �ؽ�ó / ���÷� ������Ʈ
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�
float4 main(ps_input input) : SV_TARGET
{
	input.diffuse = input.diffuse * 0.5 + 0.5f;
	// UV �����
	input.texCoord = float2(input.diffuse.x, 0.5f);

	// �ؽ�ó ���� �о����
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// ����ó�� (��ǻ��)
	//float3 diffuse = saturate(input.diffuse);	// ����Ʈ
	//float3 diffuse = input.diffuse;				// ���� ����Ʈ
	//diffuse = diffuse * 0.5f + 0.5f;			// ���� ����Ʈ
	//diffuse = pow(diffuse, 3.0f);
	//diffuse = diffuse * textureColor.rgb;

	return float4(textureColor.rgb, 1.0f);
}