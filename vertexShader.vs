#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoor;

out vec3 normal;
out vec3 worldObject;
out vec2 texCoor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * model * vec4(aPos, 1.0f);
	normal = mat3(transpose(inverse(model))) * aNormal;
	texCoor = aTexCoor;
	worldObject = vec3(model * vec4(aPos, 1.0f));
}