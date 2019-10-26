#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect texture1;
uniform float mix;

void main(void)
{
    vec2 st = gl_TexCoord[0].st;
    vec4 texturePixels = texture2DRect(texture1, st);
    vec3 color = mix - texturePixels.rgb;
    gl_FragColor = vec4(color, 1.0);
}