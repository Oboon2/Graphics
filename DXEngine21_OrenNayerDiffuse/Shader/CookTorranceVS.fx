// ��� ����
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



// ���� ���̴� �Է�
struct vs_input
{
	float4 position : POSITION;
	float2 texCoord :TEXCOORD;
	float3 normal : NORMAL;
};

// ���� ���̴� ���
struct vs_output
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 lightPosition : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};

// Entry Point(���� �Լ�)

vs_output main(vs_input input)
{
	vs_output output;


	// ���� ��ȯ
	output.position = mul(input.position, world);
	// ����Ʈ ���� ���ϱ�
    float3 lightPosition = worldLightPosition;
    //float3 lightPosition = normalize(output.position.xyz - worldLightPosition);
	// ���� ��ǥ����� ���
	output.normal = normalize(mul(input.normal, (float3x3)world));
	// ��(ī�޶�) ��ġ ����
    output.viewPosition = worldCameraPosition;
	// ���� ��ȯ ������
	output.position = mul(output.position, viewProjection);
	// UV ����
    output.texCoord = input.texCoord;


	return output;
}
