#version 440

in vec3 ReflectDir;		// The direction of the reflected ray

// The cube map
layout ( binding = 0 ) uniform samplerCube CubeMapTex;

uniform bool DrawSkyBox;		// Are we drawing the skybox ?
uniform float ReflectFactor;	// Amount of reflection
uniform vec4 MaterialColor;		// Color of the object's "Tint"


layout ( location = 0 ) out vec4 FragColor;

void main ()
{
	// Access the cube map texture
	vec4 cubeMapColor = texture ( CubeMapTex, ReflectDir );
	
	if ( DrawSkyBox )
	{
		FragColor = cubeMapColor;
	}
	
	else
	{
		FragColor = mix ( MaterialColor, cubeMapColor, ReflectFactor );
	}
}