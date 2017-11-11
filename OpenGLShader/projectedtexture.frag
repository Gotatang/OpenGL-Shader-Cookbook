#version 440

in vec3 EyeNormal;
in vec4 EyePosition;
in vec4 ProjTexCoord;

layout ( binding = 0 ) uniform sampler2D ProjectorTex;

struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform MaterialInfo Material;


struct LightInfo
{
	vec3 Intensity;
	vec4 Position;
};

uniform LightInfo Light;

layout ( location = 0 ) out vec4 FragColor;

vec3 phongModel ()
{
	vec3 s = normalize ( vec3 ( Light.Position ) - vec3 ( EyePosition ));
	vec3 v = normalize ( -EyePosition.xyz );
	vec3 r = reflect ( -s, EyeNormal );
	vec3 ambient = Light.Intensity * Material.Ka;
	
	float sDotN = max ( dot ( s, EyeNormal ), 0.0 );
	vec3 diffuse = Light.Intensity * Material.Kd * sDotN;
	vec3 specular = vec3 ( 0.0 );
	
	if ( sDotN > 0.0 )
	{
		specular = Light.Intensity * Material.Ks * pow ( max ( dot ( r, v ), 0.0 ), Material.Shininess );
	}
	
	return ambient + diffuse + specular;
}


void main ()
{
	vec3 color = phongModel ();
	
	vec4 projTexColor = vec4 ( 0.0 );

	if ( ProjTexCoord.z > 0.0 )
	{
		projTexColor = textureProj ( ProjectorTex, ProjTexCoord );
		//projTexColor = texture ( ProjectorTex, vec2 ( ProjTexCoord.xy / ProjTexCoord.w )); // Exactly the same
	}
	
	FragColor = vec4 ( color, 1.0 ) + projTexColor * 0.5;
}