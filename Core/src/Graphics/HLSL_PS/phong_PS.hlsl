struct PS_IN {
    float4 pos      : POSITION;
    float4 normal   : NORMAL;
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
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    const float3 to_camera = normalize(cam_pos - input.pos);
    const float3 to_light = normalize(-dl_dir);
    const float dl_strength = max(0.0f, dot(to_light, input.normal.xyz));

    // diffuse
    output.color = (input.pos + float4(dl_color, 1.0f)) * dl_strength;

    // specular
    const float3 r = -reflect(to_light, input.normal.xyz);
    output.color += pow(max(dot(to_camera, r), 0.0f), 10000.0f) * dl_strength;

    return output;
}