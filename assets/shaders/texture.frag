#version 100 


precision mediump float;


// Varyings
varying vec3 vVertexColor;
varying vec2 vTexCoord;

uniform sampler2D tex0;


void main()
{
	gl_FragColor = vec4(vVertexColor, 1.0);
    gl_FragColor = texture2D(tex0, vTexCoord);
}