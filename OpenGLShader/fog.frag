#version 440

in vec3 Position;
in vec3 Normal;


struct LightInfo
{
	vec4 position;
	vec3 intensity;
};

uniform LightInfo Light;


struct FogInfo
{
	float maxDist;		// Distance from the where fog in minimal
	float minDist;		// Distance from the where fog in maximal
	float density;		// For exponential fog
	vec3 color;			// Fog color
};

uniform FogInfo Fog;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;


layout ( location = 0 ) out vec4 FragColor;


vec3 ads ()
{
	vec3 n = normalize ( Normal );
	vec3 s = normalize ( vec3 ( Light.position ) - Position );
	vec3 v = normalize ( vec3 ( - Position ));
	vec3 h = normalize ( v + s );							// Halfway vector
	vec3 r = reflect ( -s, n );
	
	return Light.intensity * ( Ka + Kd * max ( dot ( s, n ), 0.0 ) + Ks * pow ( max ( dot ( h, n ), 0.0 ), Shininess ));
}


void main ()
{
	float dist = length ( Position.xyz ); // length ( Position.xyz ) : realistic & abs ( Position.z ) : unrealistic
	float fogFactor = ( Fog.maxDist - dist ) / ( Fog.maxDist - Fog.minDist );
	// float fogFactor = exp ( dist * -Fog.density );
	// float fogFactor = exp ( -Fog.density * dist * dist * Fog.density );
	
	fogFactor = clamp ( fogFactor, 0.0, 1.0 );
	
	vec3 shadeColor = ads ();
	vec3 color = mix ( Fog.color, shadeColor, fogFactor );
	
	FragColor = vec4 ( color, 1.0 );
;}