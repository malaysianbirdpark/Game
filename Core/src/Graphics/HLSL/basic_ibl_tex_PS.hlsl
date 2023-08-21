struct PS_IN {
    float4 world_pos : POSITION;
    float3 normal    : NORMAL;
    float2 texcoord  : TEXCOORD;
};

struct PS_OUT {
    float4 color : SV_TARGET;
};

cbuffer cam_position : register (b0) {
    float4 cam_pos;
}

Texture2D diffuse_map : register(t1);
TextureCube diffuse_cubemap : register(t6);
TextureCube specular_cubemap : register(t7);

SamplerState sampler0 : register(s0);

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    const float3 to_eye = cam_pos - input.world_pos;
    output.color = diffuse_map.Sample(sampler0, input.texcoord);
    output.color += diffuse_cubemap.Sample(sampler0, input.normal.xyz);
    output.color += specular_cubemap.Sample(sampler0, reflect(-to_eye, input.normal.xyz));

    return output;
}