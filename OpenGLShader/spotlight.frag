#version 440

in vec3 Position;
in vec3 Normal;

struct SpotLightInfo
{
	vec4 Position;			// Position in eye coords.
	vec3 Intensity;			// Ambient, Diffuse and Specular intensity
	vec3 Direction;			// Normalized direction of the spotlight
	float Exponent;			// Angular attenuation exponent
	float Cutoff;			// Cutoff angle ( between 0 and 90 )
};

uniform SpotLightInfo Spot;

uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

layout ( location = 0 ) out vec4 FragColor;


vec3 adsWithSpotLight ()
{
	vec3 s = normalize ( vec3 ( Spot.Position ) - Position );
	vec3 spotDir = normalize ( Spot.Direction );
	float angle = acos ( dot ( -s, spotDir ));
	float Cutoff = radians ( clamp ( Spot.Cutoff, 0.0, 90.0 ));
	
	vec3 ambient = Spot.Intensity * Ka;
	
	if ( angle < Cutoff )
	{
		float spotFactor = pow ( dot ( -s, spotDir ), Spot.Exponent );
		
		vec3 v = normalize ( vec3 ( -Position ));
		vec3 h = normalize ( v + s );
		
		return ambient + spotFactor * Spot.Intensity * ( Kd * max ( dot ( s, Normal ), 0.0 ) + Ks * pow ( max ( dot ( h, Normal), 0.0 ), Shininess ));
	}
	
	else
	{
		return ambient;
	}
}

void main ()
{
	FragColor = vec4 ( adsWithSpotLight (), 1.0 );
}