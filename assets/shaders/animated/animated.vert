#version 450

layout(location = 0) in vec2 pos;
layout(binding = 1, std430) readonly buffer TCS {
    vec2 texcoords[];
};
layout(push_constant) uniform PC {
    uint tcIndex;
} pc;

layout(location = 0) out vec2 texcoord;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    texcoord = texcoords[pc.tcIndex + gl_VertexIndex];
}
