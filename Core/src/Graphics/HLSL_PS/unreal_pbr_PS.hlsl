struct PS_IN {
    float4 world_pos : POSITION;
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
Texture2D normal_map : register(t3);
Texture2D metallic_map : register(t5);
Texture2D roughness_map : register(t6);
Texture2D ao_map : register(t7);

Texture2D previous_frame : register(t14);

TextureCube env : register(t20);
TextureCube env_specular : register(t21);
TextureCube env_diffuse : register(t22);
Texture2D brdf_lut : register(t23);

SamplerState sampler_wrap : register(s0);
SamplerState sampler_clamp : register(s1);

static const float3 Fdielectric = float3(0.04f, 0.04f, 0.04f);

float3 SchlickFresnel(const float3 F0, const float NdotV) {
    return F0 + (1.0f - F0) * pow(2.0f, (-5.55473f * NdotV - 6.98316f) * NdotV);
}

float3 UnrealPBRDiffuseIBL(const float3 normal, const float3 albedo, const float3 F0, const float3 to_camera, const float metal) {
    const float NdotV = saturate(dot(normal, to_camera));
    const float3 fresnel = SchlickFresnel(F0, NdotV);
    const float3 kd = lerp(1.0f - fresnel, 0.0f, metal);
    const float3 irradiance = env_diffuse.Sample(sampler_wrap, normal).rgb;
    
    return kd * albedo * irradiance;
}

float3 UnrealPBRSpecularIBL(const float3 normal, const float3 F0, const float3 to_camera, const float metal, const float rough) {
    const float NdotV = saturate(dot(normal, to_camera));
    const float2 specularBRDF = brdf_lut.Sample(sampler_clamp, float2(NdotV, 1.0f - rough)).rg;
    const float3 specularIrradiance = env_specular.SampleLevel(sampler_wrap, reflect(-to_camera, normal), 3.0f + rough * 5.0f).rgb;

    return (F0 * specularBRDF.r + specularBRDF.g) * specularIrradiance;
}

float3 UnrealPBRAmbient(const float3 pos, const float3 normal, const float3 to_camera, const float3 albedo, const float metal, const float rough) {
    const float3 F0 = lerp(Fdielectric, albedo, metal);
    return UnrealPBRDiffuseIBL(normal, albedo, F0, to_camera, metal) + UnrealPBRSpecularIBL(normal, F0, to_camera, metal, rough);
}

float NdfGGX(const float NdotH, const float roughness) {
    const float a = roughness * roughness;
    const float a_sq = a * a;
    const float denom = (NdotH * NdotH) * (a_sq - 1) + 1;

    return a_sq / (3.141592f * denom * denom);
}

float SchlickG1(const float NdotV, const float k) {
    return NdotV / (NdotV * (1.0f - k) + k);
}

float SchlickGGX(const float NdotL, const float NdotV, const float rough) {
    const float r = rough + 1.0f;
    const float k = (r * r) / 8.0f;
    return SchlickG1(NdotL, k) * SchlickG1(NdotV, k);
}

float3 UnrealPBRLightDiffuse(const float3 albedo, const float3 F, const float metal) {
    const float3 kd = lerp(float3(1.0f, 1.0f, 1.0f) - F, float3(0.0f, 0.0f, 0.0f), metal);
    return kd * albedo;
}

float3 UnrealPBRLightSpecular(const float3 F, const float3 NdotH, const float3 NdotL, const float3 NdotV, const float rough) {
    const float D = NdfGGX(NdotH, rough);
    const float3 G = SchlickGGX(NdotL, NdotV, rough);

    return (F * D * G) / max(1e-5, 4.0f * NdotL * NdotV);
}

float3 UnrealPBRLight(const float3 albedo, const float3 normal, const float3 to_light, const float3 to_camera, const float3 NdotL, const float metal, const float rough)
{
    const float3 h = normalize(to_light + to_camera);
    const float3 NdotH = saturate(dot(normal, h));
    const float3 NdotV = saturate(dot(normal, to_camera));
    const float3 F0 = lerp(Fdielectric, albedo, metal);
    const float3 F = SchlickFresnel(F0, saturate(dot(h, to_camera)));

    return UnrealPBRLightDiffuse(albedo, F, metal) + UnrealPBRLightSpecular(F, NdotH, NdotL, NdotV, rough);
}

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    output.color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    const float4 albedo =
        use_diffuse_map ? diffuse_map.Sample(sampler_wrap, input.texcoord) : float4(albedo_color, 1.0f);

    clip(albedo.a < 0.1f ? -1 : 1);

    if (use_emissive_map) {
        output.color = emissive_map.Sample(sampler_wrap, input.texcoord);
    }

    float3 normal = float3(0.0f, 0.0f, 0.0f);
    if (use_normal_map) {
        float3 sampled_normal = normal_map.Sample(sampler_wrap, input.texcoord).xyz;
        sampled_normal = sampled_normal * 2.0f - float3(1.0f, 1.0f, 1.0f);
        //const float3 binormal = cross(input.normal, input.tangent);
        const float3x3 tbn = float3x3(input.tangent, input.binormal, input.normal);
        normal = normalize(mul(sampled_normal, tbn));
    }
    else {
        normal = input.normal;
    }

    float metal = use_metallic_map ? metallic_map.Sample(sampler_wrap, input.texcoord).b : metallic_factor;
    metal = clamp(metal, 0.0f, 1.0f);
    float rough = use_roughness_map ? roughness_map.Sample(sampler_wrap, input.texcoord).g : roughness;
    rough = clamp(rough, Fdielectric.r, 1.0f);
    float ao = use_ao_map ? saturate(ao_map.Sample(sampler_wrap, input.texcoord).r) : 1.0f;

    if (light_type == 0)
    {
        const float3 to_camera = normalize(cam_pos - input.world_pos);

        // ambient
        output.color.rgb += float3(UnrealPBRAmbient(input.world_pos, normal, to_camera, albedo, metal, rough)) * ao * ambient_strength;
        
        // directional light
        const float3 to_light = normalize(-dl_dir);
        const float3 NdotL = saturate(dot(to_light, normal));
        output.color.rgb +=
            float3(UnrealPBRLight(albedo, normal, to_light, to_camera, NdotL, metal, rough)
            * dl_color * NdotL);
    }
    else if (light_type == 1)
    {
        output.color = previous_frame.Load(input.sv_pos);

        // point light
        const float3 to_camera = normalize(cam_pos - input.world_pos.xyz);
        float3 to_light = pl_pos - input.world_pos.xyz;
        const float dist_to_light = length(to_light);
        to_light = to_light / dist_to_light;
        const float3 NdotL = saturate(dot(to_light, normal));

        // attenuation
        const float dist_to_light_norm = 1.0f - saturate(dist_to_light * pl_reciprocal_range);
        const float att = dist_to_light_norm * dist_to_light_norm;

        output.color +=
            float4(UnrealPBRLight(albedo, normal, to_light, to_camera, NdotL, metal, rough)
            * pl_color * NdotL, 1.0f) * att;
    }

    output.color = clamp(output.color, 0.0f, 0.993f);
    return output;
}