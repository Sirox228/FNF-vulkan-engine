#version 450

layout(location = 0) in vec2 uv;
layout(binding = 0) uniform sampler2D tex;

layout(location = 0) out vec4 outColor;

const vec2 iResolution = vec2(1280, 720);

void main() {
    const vec2 texelSize = vec2(1.0) / iResolution;

    vec4 A = texture(tex, uv + texelSize * vec2(-1.0, -1.0));
    vec4 B = texture(tex, uv + texelSize * vec2( 0.0, -1.0));
    vec4 C = texture(tex, uv + texelSize * vec2( 1.0, -1.0));
    vec4 D = texture(tex, uv + texelSize * vec2(-0.5, -0.5));
    vec4 E = texture(tex, uv + texelSize * vec2( 0.5, -0.5));
    vec4 F = texture(tex, uv + texelSize * vec2(-1.0,  0.0));
    vec4 G = texture(tex, uv);
    vec4 H = texture(tex, uv + texelSize * vec2( 1.0,  0.0));
    vec4 I = texture(tex, uv + texelSize * vec2(-0.5,  0.5));
    vec4 J = texture(tex, uv + texelSize * vec2( 0.5,  0.5));
    vec4 K = texture(tex, uv + texelSize * vec2(-1.0,  1.0));
    vec4 L = texture(tex, uv + texelSize * vec2( 0.0,  1.0));
    vec4 M = texture(tex, uv + texelSize * vec2( 1.0,  1.0));

    vec2 div = (1.0 / 4.0) * vec2(0.5, 0.125);

    vec4 o = (D + E + I + J) * div.x;
    o += (A + B + G + F) * div.y;
    o += (B + C + H + G) * div.y;
    o += (F + G + L + K) * div.y;
    o += (G + H + M + L) * div.y;

    outColor = o;
}
