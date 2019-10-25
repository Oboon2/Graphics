// �Է� ����ü
struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD0;

	float3 lightDir : TEXCOORD1;
	float3 viewDir : TEXCOORD2;

	float3 normal : TEXCOORD3;
	float3 tangent : TEXCOORD4;
	float3 binormal : TEXCOORD5;
};

Texture2D diffuseMap : register(t0);
Texture2D normalMap : register(t1);
SamplerState diffuseSampler;

// ���� �Լ�
float4 main(ps_input input) : SV_TARGET
{
	// ��ȯ ��� Tangent,Binormal,Normal
	//float3x3 TBN = float3x3(normalize(input.tangent), normalize(input.binormal), normalize(input.normal));

	// ź��Ʈ ��� �о����
	float3 tangentNormal = normalMap.Sample(diffuseSampler, input.texCoord).rgb;
	tangentNormal = tangentNormal * 2 - 1;

	// ���� ��ȯ(��켱->���켱)
	float3 worldNormal = tangentNormal.x * input.tangent + tangentNormal.y * input.binormal + tangentNormal.z * input.normal;
	worldNormal = normalize(worldNormal);

	//float3 worldNormal = mul(tangentNormal, transpose(TBN));
	//worldNormal = normalize(worldNormal);

	// ���ݻ�(Diffuse)
	float3 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord).rgb;
	float3 lightDir = normalize(input.lightDir);
	float3 diffuse = dot(-lightDir, worldNormal) * 0.5f + 0.5f; // ���� ����Ʈ
	diffuse = pow(diffuse, 3.0f);
	diffuse = diffuse * texColor;


	// ���ݻ�(Specular)
	float3 specular = 0;
	float3 viewDir = normalize(input.viewDir);
	float3 halfVector = (-lightDir) + (-viewDir); // �� ���̴�
	halfVector = normalize(halfVector);

	// Glossiness
	float Glossiness = 10.0f;

	if (diffuse.x > 0)
	{
		float3 hdotn = dot(halfVector, worldNormal);
		specular = pow(saturate(hdotn), Glossiness * Glossiness);
	}

	specular = specular * texColor;

	return float4(diffuse + specular, 1.0f);

}
