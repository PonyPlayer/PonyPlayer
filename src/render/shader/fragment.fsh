#version 330 core
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;
uniform float brightness;
uniform float contrast;
uniform float saturation;

in vec2 TexCoord;
out vec4 FragColor;
const mat3 YUV_RGB_TRANSFORM = mat3(
    1,       1,        1,
    0,       -0.39465, 2.03211,
    1.13983, -0.58060, 0
);

mat4 brightnessMatrix(float brightness) {
    #define b brightness
    return mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        b, b, b, 1
    );
    #undef b
}

mat4 contrastMatrix(float contrast) {
    float minus = (1.0 - contrast) / 2.0;
    #define c contrast
    #define m minus
    return mat4(
        c, 0, 0, 0,
        0, c, 0, 0,
        0, 0, c, 0,
        m, m, m, 1
    );
    #undef u
    #undef v
}

mat4 saturationMatrix(float saturation) {
    vec3 luminance = vec3(0.3086, 0.6094, 0.0820);

    float minus = 1.0 - saturation;

    vec3 red = vec3(luminance.x * minus);
    red+= vec3(saturation, 0, 0);

    vec3 green = vec3(luminance.y * minus);
    green += vec3(0, saturation, 0);

    vec3 blue = vec3(luminance.z * minus);
    blue += vec3(0, 0, saturation);

    return mat4(
        red,     0,
        green,   0,
        blue,    0,
        0, 0, 0, 1
    );
}


void main(void) {
    vec3 yuv;
    vec3 rgb;
    yuv.x = texture(tex_y, TexCoord).r;
    yuv.y = texture(tex_u, TexCoord).r - 0.5;
    yuv.z = texture(tex_v, TexCoord).r - 0.5;
    rgb = YUV_RGB_TRANSFORM * yuv;
    FragColor = brightnessMatrix(brightness) * contrastMatrix(contrast) * saturationMatrix(saturation) * vec4(rgb, 1);
}