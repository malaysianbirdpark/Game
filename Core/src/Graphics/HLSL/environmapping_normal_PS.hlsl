struct PS_IN {
    float4 world_pos : POSITION;
    float3 normal    : NORMAL;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

TextureCube cubemap_texture : register(t7);
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    output.color = cubemap_texture.Sample(sampler0, input.normal.xyz);

    return output;
}