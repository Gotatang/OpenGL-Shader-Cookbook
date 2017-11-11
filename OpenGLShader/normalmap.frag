#version 440

// In tangent space
in vec3 LightDir;
in vec2 TexCoord;
in vec3 ViewDir;

layout ( binding = 0 ) uniform sampler2D ColorTex;
layout ( binding = 1 ) uniform sampler2D NormalMapTex;

struct LightInfo
{
	vec3 intensity;
	vec4 position;
};

uniform LightInfo Light;


struct MaterialInfo
{
	vec3 Ka;
	vec3 Ks;
	float shininess;
};

uniform MaterialInfo Material;


layout ( location = 0 ) out vec4 FragColor;

vec3 phongModel ( vec3 _norm, vec3 _diffR )
{
	vec3 r = reflect ( -LightDir, _norm );
	vec3 ambient = Light.intensity * Material.Ka;
	
	float sDotN = max ( dot ( LightDir, _norm ), 0.0 );
	
	vec3 diffuse = Light.intensity * _diffR * sDotN;
	
	vec3 spec = vec3 ( 0.0 );
	
	if ( sDotN > 0.0 )
	{
		spec = Light.intensity * Material.Ks * pow ( max ( dot ( r, ViewDir ), 0.0 ), Material.shininess );
	}
	
	return ambient + diffuse + spec;
}


void main ()
{
	// Lookup the normal from the normal map
	vec4 normal = 2.0 * texture ( NormalMapTex, TexCoord ) - 1.0; // Rescale the normal to have a -1.0 >< 1.0 range
	
	// The color texture is used as the diffuse reflectivity
	vec4 texColor = texture ( ColorTex, TexCoord );
	
	FragColor = vec4 ( phongModel ( normal.xyz, texColor.rgb ), 1.0 );
}