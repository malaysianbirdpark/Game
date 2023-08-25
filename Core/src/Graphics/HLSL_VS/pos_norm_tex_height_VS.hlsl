struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float3 world_pos : POSITION;
    float3 normal    : NORMAL;
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

cbuffer mvp : register(b0)
{
    matrix m;
    matrix mit;
    matrix mvp;
};

cbuffer options : register(b1) {
    float height_scale;
    float padding[3];
}

Texture2D height_map : register(t4);
SamplerState sampler0 : register(s0);

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    float height = height_map.SampleLevel(sampler0, input.texcoord, 0).r;
    height = height * 2.0f - 1.0f;
    input.pos += input.normal * height * height_scale;

    output.world_pos = mul(m, float4(input.pos, 1.0f)).xyz;
    output.normal = normalize(mul(mit, float4(input.normal, 1.0f)).xyz);
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}