struct ps_input
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
    float3 normal : TEXCOORD1;
};
// 픽셀 셰이더.
struct ps_output
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
};
// 텍스처 / 샘플러 스테이트
Texture2D diffuseMap;
SamplerState diffuseSampler;

// 픽셀 셰이더
ps_output main(ps_input input) : SV_TARGET // 위치값은 float4로 넘겨야함(고정값 simd)
{
    ps_output output;
    output.albedo = diffuseMap.Sample(diffuseSampler, input.texCoord);
    output.normal = float4(normalize(input.normal), 0);

    return output;
}