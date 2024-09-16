#version 450

layout(location = 0) in vec2 pos;

layout(location = 0) out vec2 texcoord;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    texcoord = clamp(pos, 0.0, 1.0);
}
