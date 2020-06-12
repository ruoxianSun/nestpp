#version 330
layout(location = 0) in vec3 Position;


uniform mat4 gMVP;
uniform vec4 specColor;
out vec4 vertColor;


void main()
{
	gl_Position = gMVP * vec4(Position,1.0);
	vertColor = specColor;
}