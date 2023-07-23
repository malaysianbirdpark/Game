struct IN {
    float4 color : COLOR;
    float4 pos : POSTION;
    float2 texcoord : TEXCOORD;
};

struct OUT {
    float4 color : SV_TARGET;
};

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

OUT main(IN input)
{
    OUT output;

    output.color = texture0.Sample(sampler0, input.texcoord);

    return output;
}