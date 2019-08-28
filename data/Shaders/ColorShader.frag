#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texture1;
uniform float r;
uniform float g;
uniform float b;

void main(void)
{
    vec2 st = gl_TexCoord[0].st;
    vec4 texturePixels = texture2DRect(texture1, st);
    vec4 color = vec4(r, g, b, 1.0);
    gl_FragColor = texturePixels * color;
}