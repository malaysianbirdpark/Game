struct IN {
    float4 color : COLOR;
    float4 pos : POSTION;
};

struct OUT {
    float4 color : SV_TARGET;
};

OUT main(IN input)
{
    OUT output;

    output.color = input.color;

    return output;
}