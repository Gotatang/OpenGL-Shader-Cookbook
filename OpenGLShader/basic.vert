#version 440

layout ( location = 0 ) in vec3 VertexPosition; // Atribute variable fix
layout ( location = 1 ) in vec3 VertexTexCoord;

out vec3 TexCoord;

void main ()
{
	TexCoord = VertexTexCoord;								// Color qui seras dispo dans le frag
	
	gl_Position = vec4 ( VertexPosition, 1.0 );				// Position qui seras transmise au frag
}