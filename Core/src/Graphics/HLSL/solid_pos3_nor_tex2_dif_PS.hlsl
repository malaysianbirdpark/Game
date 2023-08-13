struct IN {
    float4 pos      : POSITION;
    float2 texcoord : TEXCOORD;
};

struct OUT {
    float4 color    : SV_TARGET;
};

Texture2D diffuse_map : register(t1);
SamplerState sampler0 : register(s0);

OUT main(IN input)
{
    OUT output;

    output.color = diffuse_map.Sample(sampler0, input.texcoord);

    return output;
}