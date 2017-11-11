#version 440

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout ( binding = 0 ) uniform sampler2D BaseTexture;
layout ( binding = 1 ) uniform sampler2D AlphaTexture;

struct LightInfo
{
	vec3 intensity;		// A, D, S intensity
	vec4 position;		// eye coords position
};

uniform LightInfo Light;


struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform MaterialInfo Material;

layout ( location = 0 ) out vec4 FragColor;

vec3 ads ( vec3 _norm, vec3 _positon )
{
	vec3 n = normalize ( _norm );
	vec3 s = normalize ( vec3 ( Light.position ) - _positon );
	vec3 v = normalize ( vec3 ( - _positon ));
	vec3 h = normalize ( v + s );							// Halfway vector
	vec3 r = reflect ( -s, n );
	
	return Light.intensity * ( Material.Ka + Material.Kd * max ( dot ( s, n ), 0.0 ) + Material.Ks * pow ( max ( dot ( h, n ), 0.0 ), Material.Shininess ));
}

void main ()
{
	vec3 ambientDiffuse, specular;
	vec4 baseTexColor = texture ( BaseTexture, TexCoord );
	vec4 alphaTexColor = texture ( AlphaTexture, TexCoord );
	
	if ( alphaTexColor.a < 0.15 )
	{
		discard;
	}
	
	else
	{
		if ( gl_FrontFacing )
		{
			FragColor = vec4 ( ads ( Normal, Position ), 1.0 ) * baseTexColor;
		}
		
		else
		{
			FragColor = vec4 ( ads ( -Normal, Position ), 1.0 ) * baseTexColor;
		}
	}
}