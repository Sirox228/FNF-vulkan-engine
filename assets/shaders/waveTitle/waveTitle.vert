#version 450

layout(location = 0) in vec2 pos;

layout(location = 0) out vec2 fragUv;

void main() {
    gl_Position = vec4(pos, 0.0, 1.0);
    // -1 to 1 uv
    fragUv = pos;
}
