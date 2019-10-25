struct ps_input
{
	float4 position : SV_POSITION;
	float3 texCoord :TEXCOORD0;
};
// �ؽ�ó / ���÷� ������Ʈ
TextureCube cubemapTexture;
SamplerState diffuseSampler;

// �ȼ� ���̴�
float4 main(ps_input input) : SV_TARGET
{
	float3 enviroment = cubemapTexture.Sample(diffuseSampler, input.texCoord).rgb;

	return float4(enviroment, 1.0f);
}