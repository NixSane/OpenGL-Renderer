#version 450

uniform vec4 color = vec4(0.4f,0.8f,0.2f,1);

out vec4 final_colour;

void main()
{
	final_colour = color;
}