#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;


out vec3 ReflectDir;	// Reflected direction
out vec3 RefractDir;	// Transmitted direction


struct MaterialInfo
{
	float Eta;					// Patio of indices of refraction
	float ReflectionFactor;		// Percentage of reflected light
};

uniform MaterialInfo Material;

uniform bool DrawSkyBox;

uniform vec3 WorldCameraPosition;
uniform mat4 ModelMatrix;
uniform mat4 MVP;



void main ()
{
	if ( DrawSkyBox )
	{
		ReflectDir = VertexPosition;
	}
	
	else
	{
		vec3 worldPos = vec3 ( ModelMatrix * vec4 ( VertexPosition, 1.0 ));
		vec3 worldNorm = vec3 ( ModelMatrix * vec4 ( VertexNormal, 1.0 ));
		vec3 worldView = normalize ( WorldCameraPosition - worldPos );
		
		ReflectDir = reflect ( -worldView, worldNorm );
		RefractDir = refract ( -worldView, worldNorm, Material.Eta );
	}
	
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}