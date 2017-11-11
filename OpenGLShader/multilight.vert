#version 440

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexNormal;

out vec3 Color;

struct LightInfo
{
	vec4 Position;		// Light position in eye coordinates
	vec3 Intensity;		// light intensity
};

uniform LightInfo lights [ 5 ];

// Material parameters
uniform vec3 Kd;			// Diffuse reflectivity
uniform vec3 Ka;			// Ambient reflectivity
uniform vec3 Ks;			// Specular reflectivity
uniform float Shininess;	// Specular shininess factor

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;


vec3 ads ( int _lightIndex, vec4 _position, vec3 _norm )
{
	vec3 s = normalize ( vec3 ( lights [ _lightIndex ].Position - _position ));
	vec3 r = reflect ( -s, _norm );
	vec3 v = normalize ( vec3 ( -_position ));
	vec3 I = lights [ _lightIndex ].Intensity;
	
	return I * ( Ka + Kd * max ( dot ( s, _norm ), 0.0 ) + Ks * pow ( max ( dot ( r, v ), 0.0 ), Shininess ));
}

void main ()
{
	vec3 eyeNorm = normalize ( NormalMatrix * VertexNormal );
	vec4 eyePosition = ModelViewMatrix * vec4 ( VertexPosition, 1.0 );
	
	// Evaluate the lighting equation for each light
	Color = vec3 ( 0.0 );
	
	for ( int i = 0 ; i < 5 ; i ++ )
	{
		Color += ads ( i, eyePosition, eyeNorm );
	}
	
	gl_Position = MVP * vec4 ( VertexPosition, 1.0 );
}