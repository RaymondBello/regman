#version 100 


precision mediump float;


// Varyings
varying vec3 vVertexColor;


void main()
{
	gl_FragColor = vec4(vVertexColor, 1.0);
}