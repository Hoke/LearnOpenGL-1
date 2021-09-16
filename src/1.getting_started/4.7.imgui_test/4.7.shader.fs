#version 330 core
out vec4 FragColor;

uniform float colorX;
uniform float colorY;
uniform float colorZ;
uniform float colorAlpha;
void main()
{
   FragColor = vec4(colorX, colorY, colorZ, colorAlpha);
}