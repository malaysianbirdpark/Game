struct PS_IN {
    float4 world_pos : POSITION;
    float3 normal    : NORMAL;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

cbuffer cam_position : register (b0) {
    float4 cam_pos;
}

TextureCube cubemap_texture : register(t7);
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    const float3 to_eye = cam_pos - input.world_pos;
    output.color = cubemap_texture.Sample(sampler0, reflect(-to_eye, input.normal.xyz));

    return output;
}