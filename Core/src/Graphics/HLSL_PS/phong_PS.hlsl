struct PS_IN {
    float4 world_pos : POSITION;
    float3 normal    : NORMAL;
    float3 tangent   : TANGENT;
    float2 texcoord  : TEXCOORD;
    float4 sv_pos    : SV_POSITION;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

cbuffer phong_constants : register(b1) {
    float ambient_intensity;
    float diffuse_intensity;
    float specular_intensity;
    float shininess;

    bool use_diffuse_map;
    bool use_specular_map;
    bool use_normal_map;
    bool use_height_map;
}

cbuffer global_constants : register (b2) {
    float4 cam_pos;
    int light_type;
}

cbuffer hemispheric_ambient_light : register(b3)
{
    float3 amb_down_color : packoffset(c0);
    float3 amb_up_color : packoffset(c1);
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

Texture2D diffuse_map : register(t1);
Texture2D specular_map : register(t2);
Texture2D normal_map : register(t3);
Texture2D previous_frame : register(t14);

SamplerState sampler0 : register(s0);

float4 PhongAmbient(float3 normal) {
    const float up = normal.y * 0.5f + 0.5f;
    const float3 ambient = amb_down_color + up * amb_up_color;
    return float4(ambient, 1.0f);
}

float4 PhongDiffuse(float3 to_light, float3 normal) {
    const float NDotL = dot(to_light, normal);
    return max(0.0f, NDotL);
}

float4 PhongSpec(const float3 to_camera, const float3 normal, const float3 to_light, const float shininess) {
    const float3 r = reflect(-to_light, normal);
    const float NDotR = saturate(dot(r, to_camera));
    return pow(NDotR, shininess);
}

float4 PhongAmbDir(const float3 pos, const float3 normal, const float4 ambient_color, const float4 diffuse_color, const float4 specular_color) {
    const float3 to_camera = normalize(cam_pos.xyz - pos);
    const float3 to_light = normalize(-dl_dir);

    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);
    // ambient
    result.xyz += ambient_intensity * PhongAmbient(normal) * ambient_color;

    // diffuse
    result.xyz += diffuse_intensity * PhongDiffuse(to_light, normal) * dl_color * diffuse_color;

    // specular
    if (use_specular_map) {
        result.xyz += specular_intensity * PhongSpec(to_camera, normal, to_light, specular_color.a * 13) * dl_color * specular_color.rgb;
    }
    else {
        result.xyz += specular_intensity * PhongSpec(to_camera, normal, to_light, shininess) * dl_color * specular_color.rgb;
    }

    return result;
}

float4 PhongPoint(const float3 pos, const float3 normal, const float4 diffuse_color, const float4 specular_color) {
    const float3 to_camera = normalize(cam_pos.xyz - pos);
    const float3 to_light = pl_pos - pos;
    const float dist_to_light = length(to_light);
    const float3 to_light_norm = to_light / dist_to_light;

    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);

    // diffuse
    result.xyz += diffuse_intensity * PhongDiffuse(to_light_norm, normal) * diffuse_color;

    // specular
    if (use_specular_map) {
        result.xyz += specular_intensity * PhongSpec(to_camera, normal, to_light_norm,specular_color.a * 13) * specular_color.rgb;
    }
    else {
        result.xyz += specular_intensity * PhongSpec(to_camera, normal, to_light_norm, shininess) * specular_color.rgb;
    }

    // attenuation
    const float dist_to_light_norm = 1.0f - saturate(dist_to_light * pl_reciprocal_range);
    const float att = dist_to_light_norm * dist_to_light_norm;

    return float4(pl_color, 1.0f) * result * att;
}

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    const float4 ambient_color =
        use_diffuse_map ? 
            float4(diffuse_map.Sample(sampler0, input.texcoord).rgb * diffuse_map.Sample(sampler0, input.texcoord).rgb, 1.0f) :
            float4(1.0f, 1.0f, 1.0f, 1.0f);

    const float4 diffuse_color = 
        use_diffuse_map ? diffuse_map.Sample(sampler0, input.texcoord) : input.world_pos;

    clip(diffuse_color.a < 0.1f ? -1 : 1);

    float3 normal = float3(0.0f, 0.0f, 0.0f);
    if (use_normal_map) {
        float3 sampled_normal = normal_map.Sample(sampler0, input.texcoord).xyz;
        sampled_normal = sampled_normal * 2.0f - float3(1.0f, 1.0f, 1.0f);
        const float3 binormal = cross(input.normal, input.tangent);
        const float3x3 tbn = float3x3(input.tangent, binormal, input.normal);
        normal = normalize(mul(sampled_normal, tbn));
    }
    else {
        normal = input.normal;
    }

    float4 specular_color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (use_specular_map) {
        specular_color = specular_map.Sample(sampler0, input.texcoord);
    }

    // use input position for the color
    if (light_type == 0)
    {
        output.color = PhongAmbDir(input.world_pos.xyz, normal, ambient_color, diffuse_color, specular_color);
    }
    else if (light_type == 1)
    {
        output.color = previous_frame.Load(input.sv_pos);
        output.color += PhongPoint(input.world_pos.xyz, normal, diffuse_color, specular_color);
    }

    output.color = clamp(output.color, 0.0f, 0.993f);

    return output;
}