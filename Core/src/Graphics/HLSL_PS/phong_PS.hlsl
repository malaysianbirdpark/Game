struct PS_IN {
    float3 pos      : POSITION;
    float3 normal   : NORMAL;
};

struct PS_OUT {
    float4 color    : SV_TARGET;
};

cbuffer material_constant : register(b0) {
    float ambient_constant;
    float diffuse_constant;
    float specular_constant;
    float shininess;
}

cbuffer cam_position : register (b1) {
    float4 cam_pos;
}

cbuffer directional_light : register(b2)
{
    float3 dl_color;
    float padding0;
    float3 dl_dir;
    float padding1;
}

float4 PhongDirect(float3 pos, float3 normal, float4 material_color) {
    const float3 to_camera = normalize(cam_pos.xyz - pos);
    const float3 to_light = normalize(-dl_dir);
    const float3 r = reflect(normalize(dl_dir), normal);

    float4 result = float4(0.0f, 0.0f, 0.0f, 1.0f);
    // ambient
    result.xyz += ambient_constant * dl_color * material_color;

    // diffuse
    result.xyz += diffuse_constant * max(0.0f, dot(to_light, normal)) * dl_color * material_color;

    // specular
    result.xyz += specular_constant * pow(max(0.0f, dot(r, to_camera)), shininess) * dl_color * material_color;

    return result;
}

PS_OUT main(PS_IN input)
{
    PS_OUT output;

    // use input position for the color
    output.color = PhongDirect(input.pos, input.normal, float4(input.pos, 1.0f));

    return output;
}