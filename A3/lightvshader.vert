#version 330

layout (location = 0) in vec3 pos;

uniform mat4 lightmodel;

void main()
{
	gl_Position = lightmodel * vec4(pos.x, pos.y, pos.z, 1.0f);
}
