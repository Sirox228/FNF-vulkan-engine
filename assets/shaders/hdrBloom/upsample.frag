#version 450

layout(location = 0) in vec2 uv;
layout(set = 0, binding = 0) uniform sampler2D tex;
layout(set = 1, binding = 1) uniform sampler2D downsampledTex;

layout(location = 0) out vec4 outColor;

const vec2 iResolution = vec2(1280.0, 720.0);

vec3 tonemapAces(const vec3 hdrColor) {
    mat3 input_aces_transform = mat3(
        0.59719, 0.07600, 0.02840,
        0.35458, 0.90834, 0.13383,
        0.04823, 0.01566, 0.83777
    );

    mat3 output_aces_transform = mat3(
        1.60475, -0.10208, -0.00327,
        -0.53108,  1.10813, -0.07276,
        -0.07367, -0.00605,  1.07602
    );

    vec3 v = input_aces_transform * hdrColor;
    vec3 a = v * (v + 0.0245786) - 0.000090537;
    vec3 b = v * (0.983729 * v + 0.4329510) + 0.238081;
    return output_aces_transform * (a / b);
}

void main() {
    const vec2 texelSize = vec2(1.0) / iResolution;
    const vec4 d = texelSize.xyxy * vec4(1.0, 1.0, -1.0, 0.0) * 4.0;

    vec3 color;
    color =  texture(downsampledTex, uv - d.xy).rgb;
    color += texture(downsampledTex, uv - d.wy).rgb * 2.0;
    color += texture(downsampledTex, uv - d.zy).rgb;

    color += texture(downsampledTex, uv + d.zw).rgb * 2.0;
    color += texture(downsampledTex, uv).rgb * 4.0;
    color += texture(downsampledTex, uv + d.xw).rgb * 2.0;

    color += texture(downsampledTex, uv + d.zy).rgb;
    color += texture(downsampledTex, uv + d.wy).rgb * 2.0;
    color += texture(downsampledTex, uv + d.xy).rgb;

    color *= (1.0 / 16.0);

    color += texture(tex, uv).rgb;
    color = tonemapAces(color);
    outColor = vec4(color, 1.0);
}
