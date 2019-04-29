#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 model;


out vec4 vCol;
out vec3 Normal;
out vec3 fragPos;

void main()
{
	gl_Position = model * vec4(pos.x, pos.y, pos.z, 1.0f);
	vCol = vec4(color.x,color.y,color.z,1.0);
	vec4 worldPos = (model*vec4(pos,1.0));
	fragPos = vec3(worldPos.x,worldPos.y,worldPos.z);
	Normal = mat3(transpose(inverse(model)))*color;
}
