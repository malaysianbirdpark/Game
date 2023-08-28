struct PS_IN {
    float3 world_pos : POSITION;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float3 binormal  : BINORMAL;
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
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

cbuffer cam_position : register (b2) {
    float4 cam_pos;
    int light_type : packoffset(c1);
}

cbuffer directional_light : register(b4)
{
    float3 dl_color : packoffset(c0);
    float3 dl_dir : packoffset(c1);
}

cbuffer point_light : register(b5) {
    float3 pl_pos : packoffset(c0);
    float3 pl_color : packoffset(c1);
    float pl_reciprocal_range : packoffset(c2);
}

Texture2D emissive_map : register(t0);
Texture2D diffuse_map : register(t1);
Texture2D specular_map : register(t2);
Texture2D normal_map : register(t3);
Texture2D metallic_map : register(t5);
Texture2D roughness_map : register(t6);
Texture2D ao_map : register(t7);
Texture2D brdf_lut : register(t8);

Texture2D previous_frame : register(t14);

Texture2D env_map : register(t20);
Texture2D env_specular : register(t21);
Texture2D env_diffuse : register(t22);

SamplerState sampler_wrap : register(s0);
SamplerState sampler_clamp : register(s1);

static const float3 Fdielectric = 0.04;

float3 SchlickFresnel(float3 fresnel_constant, float NdotV) {
    return fresnel_constant + (1.0f - fresnel_constant) * pow(2.0f, (-5.55473f * NdotV - 6.98316f) * NdotV);
}

float3 UnrealPBRDiffuseIBL(float3 normal, float3 color, float3 to_camera, float metallic_factor) {
    const float3 fresnel_constant = lerp(Fdielectric, color, metallic_factor);
    const float NdotV = saturate(dot(normal, to_camera));
    const float3 fresnel = SchlickFresnel(fresnel_constant, NdotV);
    const float3 kd = lerp(1.0f - fresnel, 0.0f, metallic_factor);
    const float3 irradiance = env_diffuse.Sample(sampler_wrap, normal).rgb;
    
    return kd * color * irradiance;
}

float3 UnrealPBRSpecularIBL(float3 normal, float3 to_camera, float3 color, float metallic, float roughness) {
    const float NdotV = saturate(dot(normal, to_camera));
    const float2 specularBRDF = brdf_lut.Sample(sampler_clamp, float2(NdotV, 1.0f - roughness)).rg;
    const float3 specularIrradiance = env_specular.SampleLevel(sampler_wrap, reflect(-to_camera, normal), roughness * 10.0f).rgb;
    const float3 F0 = lerp(Fdielectric, color, metallic);

    return (F0 * specularBRDF.x * specularBRDF.y) * specularIrradiance;
}

float4 UnrealPBRAmbient() {
}

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    const float4 diffuse_color =
        use_diffuse_map ? diffuse_map.Sample(sampler_wrap, input.texcoord) : diffuse_color;

    float3 normal = float3(0.0f, 0.0f, 0.0f);
    if (use_normal_map) {
        float4 sampled_normal = normal_map.Sample(sampler_wrap, input.texcoord);
        sampled_normal = sampled_normal * 2.0f - 1.0f;
        const float4x4 tbn = float4x4(float4(input.tangent, 0.0f), float4(input.binormal, 0.0f), float4(input.normal, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
        normal = normalize(mul(tbn, sampled_normal)).xyz;
    }
    else {
        normal = input.normal;
    }

    if (light_type == 0) {
        // ambient
        
        // directional light
    }
    else if (light_type == 1) {
        // point light
    }

    return output;
}