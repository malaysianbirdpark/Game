struct IN {
    float3 pos : POSITION;
};

struct OUT {
    float4 color : SV_TARGET;
};

TextureCube  cubemap_texture : register(t20);
SamplerState sampler0 : register(s0);

OUT main(IN input)
{
    OUT output;

    output.color = cubemap_texture.Sample(sampler0, input.pos.xyz);

    return output;
}