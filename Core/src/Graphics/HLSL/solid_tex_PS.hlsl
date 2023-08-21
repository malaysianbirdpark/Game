struct PS_IN {
    float2 texcoord : TEXCOORD;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

Texture2D diffuse_map : register(t1);
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input) : SV_TARGET
{
    PS_OUT output;

    output.color = diffuse_map.Sample(sampler0, input.texcoord);

    return output;
}