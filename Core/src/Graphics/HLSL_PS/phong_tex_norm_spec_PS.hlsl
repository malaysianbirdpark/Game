struct PS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

cbuffer material_constant : register(b0) {
    float ambient_constant;
    float diffuse_constant;
    float specular_constant;
    float shininess;
}

cbuffer cam_position : register (b1) {
    float4 cam_pos;
}

cbuffer directional_light : register(b2)
{
    float3 dl_color;
    float padding0;
    float3 dl_dir;
    float padding1;
}

Texture2D diffuse_map : register(t1);
Texture2D normal_map : register(t3);
Texture2D specular_map : register(t2);
SamplerState sampler0 : register(s0);

float4 PhongDirect(float3 pos, float3 normal, float4 material_color, float4 specular_color) {
    const float3 to_camera = normalize(cam_pos.xyz - pos);
    const float3 to_light = normalize(-dl_dir);
    const float3 r = reflect(normalize(dl_dir), normal);

    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);
    // ambient
    result.xyz += ambient_constant * dl_color * material_color;

    // diffuse
    result.xyz += diffuse_constant * max(0.0f, dot(to_light, normal)) * dl_color * material_color;

    // specular
    result.xyz += specular_constant * pow(max(0.0f, dot(r, to_camera)), shininess) * dl_color * specular_color;

    return result;
}

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    float3 sampled_normal = normal_map.Sample(sampler0, input.texcoord);
    sampled_normal = sampled_normal * 2.0f - 1.0f;
    const float4x4 tbn = float4x4(float4(input.tangent, 0.0f), float4(input.binormal, 0.0f), float4(input.normal, 0.0f), float4(0.0f, 0.0f, 0.0f, 1.0f));
    const float3 normal = normalize(mul(tbn, sampled_normal)).xyz;

    const float4 sampled_specular = specular_map.Sample(sampler0, input.texcoord);
    output.color = PhongDirect(input.pos, normal, diffuse_map.Sample(sampler0, input.texcoord), sampled_specular);

    return output;
}