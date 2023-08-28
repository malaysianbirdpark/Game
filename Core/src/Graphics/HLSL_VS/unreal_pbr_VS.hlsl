struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float3 world_pos : POSITION;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 binormal  : BINORMAL;
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

cbuffer mvp : register(b0)
{
    matrix m;
    matrix mit;
    matrix mvp;
};

cbuffer unreal_pbr_constants : register(b1) {
    float3 albedo_color;
    float metallic_factor;
    float roughness;

    bool use_emissive_map;
    bool use_diffuse_map;
    bool use_specular_map;
    bool use_normal_map;
    bool use_height_map;
    bool use_matallic_map;
    bool use_roughness_map;
    bool use_ao_map;
    bool use_brdf_lut;
}

cbuffer vs_constant : register(b2) {
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

    output.world_pos = mul(m, float4(input.pos, 1.0f)).xyz;
    output.normal = normalize(mul(mit, float4(input.normal, 1.0f)).xyz);
    output.tangent = normalize(input.tangent);
    output.binormal = normalize(input.binormal);
    output.texcoord = input.texcoord;
    output.sv_pos = mul(mvp, float4(input.pos, 1.0f));

	return output;
}