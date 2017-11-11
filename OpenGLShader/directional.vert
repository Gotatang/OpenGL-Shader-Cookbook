#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;

out vec3 Color;

uniform vec4 LightPosition;
uniform vec3 LightIntensity;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

vec3 ads ( vec4 _position, vec3 _norm )
{
	vec3 s;
	
	if ( LightPosition.w == 0.0 )
	{
		s = normalize ( vec3 ( LightPosition ));
	}
	
	else
	{
		s = normalize ( vec3 ( LightPosition - _position ));
	}
	
	vec3 v = normalize ( vec3 ( -_position ));
	vec3 r = reflect ( -s, _norm );
	
	return LightIntensity * ( Ka + Kd * max ( dot ( s, _norm ), 0.0 ) + Ks * pow ( max ( dot ( r, v ), 0.0 ), Shininess ));
}

void main ()
{
	vec3 eyeNorm = normalize ( NormalMatrix * VertexNormal );
	vec4 eyePosition = ModelViewMatrix * vec4 ( VertexPosition, 1.0 );
	
	// Evaluate the lighting equation
	Color = ads ( eyePosition, eyeNorm );
	
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}