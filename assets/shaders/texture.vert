#version 100

// Attributes
attribute vec3 aPosition;
attribute vec3 aColor;
attribute vec2 aTex;

// Uniforms
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

// Varyings
varying vec3 vVertexColor;
varying vec2 vTexCoord;


void main()
{
    vVertexColor = aColor;
    vTexCoord = aTex;

    gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition, 1.0);
}