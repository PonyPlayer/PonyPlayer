#version 330 core
layout (location = 0) in vec3 aPos;      // 位置坐标
layout (location = 1) in vec2 aTexCoord; // 纹理坐标
uniform mat4 view;
out vec2 TexCoord;
void main() {
    gl_Position = view * vec4(aPos, 1.0F);
    TexCoord = aTexCoord;

}
