#version 430 core

out vec4 Color;
in vec3 oColor;

uniform float clr;

void main()
{
   Color = vec4(oColor.x, normalize(oColor.y + clr), oColor.z, 1.0);
}