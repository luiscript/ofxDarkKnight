#version 120
#extension GL_ARB_texture_rectangle : enable

uniform vec2 resolution;
uniform sampler2DRect tex;
uniform float vertical;
uniform float horizontal;

void main() {
  vec2 normSrcCoord = gl_TexCoord[0].st;
	if (normSrcCoord.x > resolution.x * vertical)	{
		normSrcCoord.x = (resolution.x-normSrcCoord.x);
	}

	if ((normSrcCoord.y > resolution.y * horizontal))	{
		normSrcCoord.y = (resolution.y-normSrcCoord.y);
	}
	
	gl_FragColor = texture2DRect(tex, normSrcCoord);
}
