struct PS_IN {
    float3 world_pos : POSITION;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    output.color = float4(input.world_pos, 1.0f);

    return output;
}