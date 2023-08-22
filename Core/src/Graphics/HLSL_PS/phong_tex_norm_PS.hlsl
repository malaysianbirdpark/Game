struct PS_IN {
    float4 pos      : POSITION;
    float3 normal   : NORMAL;
    float3 tangent  : TANGENT;
    float3 binormal : BINORMAL;
    float2 texcoord : TEXCOORD;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

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
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    output.color = float4(0.0f, 0.0f, 1.0f, 1.0f);

    //const float3 to_camera = normalize(cam_pos - input.pos);
    //const float3 to_light = normalize(-dl_dir);

    //float3 normal = normalize(normal_map.Sample(sampler0, input.texcoord).xyz);
    //normal = normal * 2.0f - 1.0f;
    //const float3x3 tbn = float3x3(input.normal, input.tangent, input.binormal);

    //const float3 normal_world = mul(tbn, normal);
    //const float dl_strength = max(0.0f, dot(to_light, normal_world));

    // diffuse
    //output.color = (diffuse_map.Sample(sampler0, input.texcoord) + float4(dl_color, 1.0f)) * dl_strength;

    // specular
    //const float3 r = -reflect(to_light, input.normal);
    //output.color += pow(max(dot(to_camera, r), 0.0f), 10000.0f) * dl_strength;

    return output;
}