struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float4 world_pos : POSITION;
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
    bool use_height_map;
}

cbuffer global_constants : register (b2) {
    matrix vp;
    float4 cam_pos;
    int light_type;
}

cbuffer vs_constants : register(b3) {
    float height_scale;
}

Texture2D height_map : register(t4);
SamplerState sampler0 : register(s0);

VS_OUT main(VS_IN input)
{
    VS_OUT output;

    if (use_height_map) {
        float height = height_map.SampleLevel(sampler0, input.texcoord, 0).r;
        height = height * 2.0f - 1.0f;
        input.pos += input.normal * height * height_scale;
    }

    output.world_pos = mul(float4(input.pos, 1.0f), m);
    output.normal = normalize(mul(input.normal, (float3x3) mit).xyz);
    output.texcoord = input.texcoord;
    output.sv_pos = mul(float4(input.pos, 1.0f), mvp);

	return output;
}