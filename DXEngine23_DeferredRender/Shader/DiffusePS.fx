struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord :TEXCOORD0;
	float3 diffuse : TEXCOORD1;
};
// 텍스처 / 샘플러 스테이트
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더
float4 main(ps_input input) : SV_TARGET
{
	// 텍스처 색상 읽어오기
	float4 textureColor = diffuseMap.Sample(diffuseSampler, input.texCoord);

	// 음영처리 (디퓨즈)
	//float3 diffuse = saturate(input.diffuse);	// 램버트
	float3 diffuse = input.diffuse;				// 하프 램버트
	diffuse = diffuse * 0.5f + 0.5f;			// 하프 램버트
	diffuse = pow(diffuse, 3.0f);
	diffuse = diffuse * textureColor.rgb;

	return float4(diffuse, 1.0f);
}