struct PS_IN {
    float4 world_pos : POSITION;
    float3 normal    : NORMAL;
    float4 sv_pos    : SV_POSITION;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

Texture2D input_frame : register(t30);
TextureCube cubemap_texture : register(t20);
SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    output.color = input_frame.Load(input.sv_pos) * 0.8f + cubemap_texture.Sample(sampler0, input.normal.xyz) * 0.2f;

    return output;
}