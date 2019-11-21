#version 120
#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect tex1;
uniform float mixer;

void main(void)
{
    vec2 st = gl_TexCoord[0].st;
    vec4 tp = texture2DRect(tex1, st);
    vec3 inv = vec3(1.0 - tp.r, 1.0 - tp.g, 1.0 - tp.b);
    vec3 color = mix(tp.rgb, inv, mixer)  ;
    gl_FragColor = vec4(color, 1.0);
}