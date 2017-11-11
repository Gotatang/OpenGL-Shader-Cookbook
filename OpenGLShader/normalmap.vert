#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;
layout ( location = 2 ) in vec2 VertexTexCoord;
layout ( location = 3 ) in vec4 VertexTangent;

struct LightInfo
{
	vec4 position;
	vec3 intensity;
};

uniform LightInfo Light;

out vec3 LightDir;
out vec2 TexCoord;
out vec3 ViewDir;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;


void main ()
{
	// Transform normal and tangent to eye space
	vec3 norm = normalize ( NormalMatrix * VertexNormal );
	vec3 tang = normalize ( NormalMatrix * vec3 ( VertexTangent ));
	
	// Compute the binormal vector
	vec3 binormal = normalize ( cross ( norm, tang )) * VertexTangent.w; // <- determine the handedness
	
	// Matrix for transformation to tangent space
	mat3 toObjectLocal = mat3 ( tang.x, binormal.x, norm.x, tang.y, binormal.y, norm.y, tang.z, binormal.z, norm.z );
	
	// Get the position in eye coordinates
	vec3 posEye = vec3 ( ModelViewMatrix * vec4 ( VertexPosition, 1.0 ));
	
	// Transform light direction
	LightDir = normalize ( toObjectLocal * ( Light.position.xyz - posEye ));
	
	ViewDir = toObjectLocal * normalize ( -posEye );
	
	// Pass along the texture coordinates
	TexCoord = VertexTexCoord;
	
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}