#version 330


uniform vec4 lightColor;

out vec4 fragcolor;

void main()
{
	fragcolor = lightColor;
}