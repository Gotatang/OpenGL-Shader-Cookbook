#version 440

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout ( binding = 0 ) uniform sampler2D Tex1;

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

void phongModel ( vec3 _pos, vec3 _norm, out vec3 _ambientDiffuse, out vec3 _specular )
{
	vec3 s = normalize ( vec3 ( Light.position ) - _pos );
	vec3 n = normalize ( Normal );
	vec3 v = normalize ( vec3 ( -_pos ));
	vec3 h = normalize ( v + s );
	
	_ambientDiffuse = Light.intensity * ( Material.Ka + Material.Kd * max ( dot ( s, n ), 0.0 ));
	
	_specular = Light.intensity * ( pow ( max ( dot ( h, n ), 0.0 ), Material.Shininess ));
}

void main ()
{
	vec3 ambientDiffuse, specular;
	vec4 texColor = texture ( Tex1, TexCoord );
	
	phongModel ( Position, Normal, ambientDiffuse, specular );
	
	FragColor = vec4 ( ambientDiffuse, 1.0 ) * texColor + vec4 ( specular, 1.0 );
	//FragColor = mix (  vec4 ( specular, 1.0 ), vec4 ( ambientDiffuse, 1.0 ), texColor); // Seems the same
}