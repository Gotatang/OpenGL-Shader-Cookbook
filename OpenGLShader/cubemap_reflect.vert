#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;
layout ( location = 2 ) in vec2 VertexTexCoord;

out vec3 ReflectDir;		// The direction of the reflected ray

uniform bool DrawSkyBox;	// Are we drawing the skybox ?

uniform vec3 WorldCameraPosition;

uniform mat4 ModelViewMatrix;
uniform mat4 MVP;


void main ()
{
	if ( DrawSkyBox )
	{
		ReflectDir = VertexPosition;
	}
	
	else
	{
		// Compute the reflected direction in world coordinates
		vec3 worldPos = vec3 ( ModelViewMatrix * vec4 ( VertexPosition, 1.0 ));
		
		vec3 worldNorm = vec3 ( ModelViewMatrix * vec4 ( VertexNormal, 0.0 ));
		
		vec3 worldView = normalize ( WorldCameraPosition - worldPos );
		
		ReflectDir = reflect ( -worldView, worldNorm );
	}
	
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}