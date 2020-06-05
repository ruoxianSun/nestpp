#version 330
layout(location = 0) in vec3 position;

uniform vec3 specColor;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec4 vertColor;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	vertColor = vec4(specColor,1.0);
}