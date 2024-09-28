#version 450

// -1 to 1 uv
layout(location = 0) in vec2 uv;
layout(push_constant) uniform PushConstant {
    uint ticks;
} pushConstant;

layout(location = 0) out vec4 outColor;

const vec3 waveColor = vec3(0.144, 0.741, 1.0);

void main() {
    float alpha = clamp(uv.y - 0.7, 0.0, 1.0);

    // wave moving
    alpha *= abs(sin(pushConstant.ticks / 400.0) / 3.0) + 1.0;

    outColor = vec4(waveColor, alpha);
}
