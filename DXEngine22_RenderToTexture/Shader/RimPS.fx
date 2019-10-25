struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD0;
	float3 diffuse : TEXCOORD1;

	float3 viewDir : TEXCOORD2;
	float3 normal : TEXCOORD3;
};
// �ؽ�ó / ���÷� ������Ʈ
Texture2D diffuseMap;
SamplerState diffuseSampler;

// �ȼ� ���̴�
float4 main(ps_input input) : SV_TARGET
{
	// �ؽ�ó ���� �о����
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// ����ó�� (��ǻ��)
	//float3 diffuse = saturate(input.diffuse);	// ����Ʈ
	float3 diffuse = input.diffuse;				// ����Ʈ
	diffuse = diffuse * 0.5f + 0.5f;			// ���� ����Ʈ
	diffuse = pow(diffuse, 3.0f);
	diffuse = diffuse * textureColor.rgb;

	float3 normal = normalize(input.normal);
	float3 rimColor = float3(1.0f, 0.95f, 0.85f);
	//float3 rim = saturate(dot(-input.viewDir, normal));
	float3 rim = abs(dot(-input.viewDir, normal));

	float rimConstant = 5.0f;
	rim = pow(1.0f - rim, rimConstant) * rimColor;

	return float4(rim + diffuse, 1.0f);
}