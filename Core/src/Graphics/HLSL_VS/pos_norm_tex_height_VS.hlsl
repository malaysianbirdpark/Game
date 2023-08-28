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

cbuffer texture_options : register(b1) {
    bool use_diffuse_map;
    bool use_specular_map;
    bool use_normal_map;
    bool use_height_map;
    float padding1[3];
}

Texture2D height_map : register(t4);
SamplerState sampler0 : register(s0);

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    if (use_height_map) {
        float height = height_map.SampleLevel(sampler0, input.texcoord, 0).r;
        height = height * 2.0f - 1.0f;
        input.pos += input.normal * height;
    }

    output.world_pos = mul(m, float4(input.pos, 1.0f)).xyz;
    output.normal = normalize(mul(mit, float4(input.normal, 1.0f)).xyz);
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}