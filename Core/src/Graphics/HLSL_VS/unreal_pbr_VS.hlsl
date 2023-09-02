struct VS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUT {
    float4 world_pos : POSITION;
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
    float ambient_strength;
    bool use_emissive_map;
    bool use_diffuse_map;
    bool use_normal_map;
    bool use_height_map;
    bool use_metallic_map;
    bool use_roughness_map;
    bool use_ao_map;
}

cbuffer global_constants : register (b2) {
    float4 cam_pos;
    int light_type;
}

cbuffer vs_constant : register(b3) {
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
    output.normal = normalize(mul(input.normal, (float3x3) mit));
    output.tangent = normalize(mul(input.tangent, (float3x3) m));
    output.binormal = normalize(mul(input.binormal, (float3x3) m));
    output.texcoord = input.texcoord;
    output.sv_pos = mul(float4(input.pos, 1.0f), mvp);

	return output;
}