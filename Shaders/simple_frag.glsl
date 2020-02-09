#version 450

uniform vec4 color = vec4(1,1,1,1);

out vec4 final_colour;

void main()
{
	final_colour = color;
}