#version 100

// Attributes
attribute vec3 aPosition;
attribute vec3 aColor;

// Uniforms
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

// Varyings
varying vec3 vVertexColor;


void main()
{
    vVertexColor = aColor;

    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
}