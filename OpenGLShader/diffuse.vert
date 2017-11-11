#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 LightPosition;		// Light position in eye coordinates
uniform vec3 Kd;				// Diffuse reflectivity
uniform vec3 Ld;				// Light source intensity

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
//uniform mat4 ProjectionMatrix;
uniform mat4 MVP;				// Projection * ModelView


void main ()
{
	// Convert normal and position to eye coord
	vec3 tNorm = normalize ( NormalMatrix * VertexNormal );
	vec4 eyeCoords = ModelViewMatrix * vec4 ( VertexPosition, 1.0 ); // Place le vertex en eye coord
	
	vec3 s  = normalize ( vec3 ( LightPosition - eyeCoords ));
	
	// Diffuse shading equation
	LightIntensity =  Ld * Kd * max ( dot ( s, tNorm ), 0.0 );
	
	// Convert position to clip coordinates and pass along
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}