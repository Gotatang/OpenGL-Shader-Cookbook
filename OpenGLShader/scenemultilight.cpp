#include "scenemultilight.h"

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
using glm::vec4;


SceneMultiLight::SceneMultiLight ()
{}

SceneMultiLight::~SceneMultiLight ()
{
	delete plane;
}


void SceneMultiLight::Init ()
{
	try
	{
		shader.CompileShader ( "multilight.vert" );
		shader.CompileShader ( "multilight.frag" );
		shader.Link ();
		shader.Validate ();
		shader.Use ();
	}

	catch ( GLSLProgramException &e )
	{
		cerr << e.what () << endl;
		exit ( EXIT_FAILURE );
	}

	glEnable ( GL_DEPTH_TEST );

	plane = new VBOPlane ( 10.f, 10.f, 100, 100 );
	mesh = new VBOMesh ( "../Media/Model/pig_triangulated.obj", true );

	view = glm::lookAt ( vec3 ( 0.5f, 0.75f, 0.75f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 70.f ), ( float ) 800 / 600, 0.3f, 100.f );


	float x, z;

	for ( int i = 0 ; i < 5 ; i ++ )
	{
		std::stringstream name;
		name << "lights[" << i << "].Position";
		x = 2.f * cosf (( glm::two_pi <float> () / 5 ) * i );
		z = 2.f * sinf (( glm::two_pi <float> () / 5 ) * i );
		shader.setUniform ( name.str ().c_str (), view * vec4 ( x, 1.2f, z + 1.f, 1.f ));
	}

	shader.setUniform ( "lights[0].Intensity", vec3( 0.0f, 0.8f, 0.8f ));
    shader.setUniform ( "lights[1].Intensity", vec3( 0.0f, 0.0f, 0.8f ));
    shader.setUniform ( "lights[2].Intensity", vec3( 0.8f, 0.0f, 0.0f ));
    shader.setUniform ( "lights[3].Intensity", vec3( 0.0f, 0.8f, 0.0f ));
    shader.setUniform ( "lights[4].Intensity", vec3( 0.8f, 0.8f, 0.8f ));
}

void SceneMultiLight::Update ( float t )
{}

void SceneMultiLight::Render ()
{
	shader.setUniform( "Kd", 0.4f, 0.4f, 0.4f );
    shader.setUniform( "Ks", 0.9f, 0.9f, 0.9f );
    shader.setUniform( "Ka", 0.1f, 0.1f, 0.1f );
    shader.setUniform( "Shininess", 180.0f );

	model = mat4 ( 1.f );
	model *= glm::rotate ( glm::radians ( 90.f ), vec3 ( 0.f, 1.f, 0.f ));

	setMatrices ();
	mesh->Render ();


	shader.setUniform( "Kd", 0.1f, 0.1f, 0.1f );
    shader.setUniform( "Ks", 0.9f, 0.9f, 0.9f );
    shader.setUniform( "Ka", 0.1f, 0.1f, 0.1f );
    shader.setUniform( "Shininess", 180.0f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -0.45f, 0.f ));

	setMatrices ();
	plane->Render ();
}


void SceneMultiLight::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}