struct PS_IN {
    float3 world_pos : POSITION;
    float3 normal    : NORMAL;
    float2 texcoord  : TEXCOORD;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

cbuffer texture_options : register(b1) {
    bool use_diffuse_map : packoffset(c0);
    bool use_height_map : packoffset(c1);
}

Texture2D diffuse_map : register(t1);
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    if (use_diffuse_map) {
        output.color = diffuse_map.Sample(sampler0, input.texcoord);
    }
    else {
        output.color = float4(input.world_pos, 1.0f);
    }

    return output;
}