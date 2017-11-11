#include "scenesamplerobj.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

#include <iostream>
using std::cout;
using std::endl;

SceneSamplerObj::SceneSamplerObj () : angle ( 0.f )
{}

SceneSamplerObj::~SceneSamplerObj ()
{
	delete plane;
}


void SceneSamplerObj::Init ()
{
	try
	{
		shader.CompileShader ( "samplerobj.vert" );
		shader.CompileShader ( "samplerobj.frag" );
		shader.Link ();
		shader.Validate ();
		shader.Use ();
	}

	catch ( GLSLProgramException &e )
	{
		cout << e.what () << endl;
		exit ( EXIT_FAILURE );
	}

	glEnable ( GL_DEPTH_TEST );
	glClearColor ( 0.9f, 0.9f, 0.9f, 1.f );
	
	plane = new VBOPlane ( 10.f, 10.f, 1, 1 );

	view = glm::lookAt ( vec3 ( 0.f, .1f, 6.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));
	
	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	shader.setUniform ( "Light.Intensity", vec3 ( 1.f, 1.f, 1.f ));

	// A simple 128x128 checkerboard texture
	GLint w = 128, h = 128, checksize = 4;
	GLubyte* data = new GLubyte [ w * h * 4 ];

	for ( int r = 0 ; r < h ; r ++ )
	{
		for ( int c = 0 ; c < w ; c ++ )
		{
			GLubyte color = 0;

			if ((( c / checksize ) + ( r / checksize )) % 2 == 0 )
			{
				color = 0;
			}

			else
			{
				color = 255;
			}

			data [( r * w + c ) * 4 + 0 ] = color;
			data [( r * w + c ) * 4 + 1 ] = color;
			data [( r * w + c ) * 4 + 2 ] = color;
			data [( r * w + c ) * 4 + 3 ] = 255;
		}
	}

	//Create the texture object
	GLuint handleTexture;

	glGenTextures ( 1, &handleTexture );
	glBindTexture ( GL_TEXTURE_2D, handleTexture );

	glTexStorage2D ( GL_TEXTURE_2D, 1, GL_RGBA8, w, h );		// 2 derneier arguments =  largeur et longueur de la texture
	glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

	delete [] data;

	// Bind texture object
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D, handleTexture );

	// Create some sampler objects
	GLuint samplers [ 2 ];

	glGenSamplers ( 2, samplers );

	linearSampler = samplers [ 0 ];
	nearestSampler = samplers [ 1 ];

	// Set up linear sampler
	glSamplerParameteri ( linearSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glSamplerParameteri ( linearSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	// Set up nearest sampler
	glSamplerParameteri ( nearestSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glSamplerParameteri ( nearestSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	// Bind the sampler object to the same texture unit and set the sampler uniform
	shader.setUniform ( "Tex1", 0 );
}

void SceneSamplerObj::Update ( float t )
{}

void SceneSamplerObj::Render ()
{
	shader.setUniform ( "Light.Position", vec4 ( 0.f, 20.f, 0.f, 1.f ));
	shader.setUniform ( "Material.Kd", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Material.Ks", 0.95f, 0.95f, 0.95f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 100.f );

	mat4 rot = glm::rotate ( mat4 (), glm::radians ( 10.f ), vec3 ( 1.f, 0.f, 0.f ));

	model = glm::translate ( rot, vec3 ( -5.01f, 0.f, 0.f ));
	setMatrices ();

	glBindSampler ( 0, nearestSampler );
	plane->Render ();

	model = glm::translate ( rot, vec3 ( 5.01f, 0.f, 0.f ));
	setMatrices ();

	glBindSampler ( 0, linearSampler );
	plane->Render ();
}


void SceneSamplerObj::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}