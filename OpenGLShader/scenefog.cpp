#include "scenefog.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"


SceneFog::SceneFog () : fPrev ( 0.f )
{}

SceneFog::~SceneFog ()
{
	delete plane;
	delete teapot;
	delete torus;
}


void SceneFog::Init ()
{
	try
	{
		shader.CompileShader ( "fog.vert" );
		shader.CompileShader ( "fog.frag" );
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

	plane = new VBOPlane ( 50.f, 50.f, 1, 1 );
	teapot = new VBOTeapot ( 14, mat4 ( 1.f ));
	torus = new VBOTorus ( 1.75f * 0.75f, 0.75f, 50, 50 );

	view = glm::lookAt ( vec3 ( 0.f, 4.f, 6.f ), vec3 ( 0.f, 2.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	angle = 0.f;

	shader.setUniform ( "Light.intensity", vec3 ( 0.9f, 0.9f, 0.9f ));
	shader.setUniform ( "Fog.maxDist", 30.f );
	shader.setUniform ( "Fog.minDist", 1.f );
	shader.setUniform ( "Fog.color", vec3 ( 0.5f, 0.5f, 0.5f ));
	shader.setUniform ( "Fog.density", 0.05f );
}

void SceneFog::Update ( float t )
{
	float deltaT = t - fPrev;

	if ( fPrev == 0.f )
	{
		deltaT = 0.f;
	}

	fPrev = t;

	angle += 1.f * deltaT;

	if ( angle > glm::two_pi <float> ())
	{
		angle -= glm::two_pi <float> ();
	}
}

void SceneFog::Render ()
{
	vec4 lightPos = vec4 ( 10.f * cos ( angle ), 10.f, 10.f * sin ( angle ), 1.f );
	shader.setUniform ( "Light.position", view * lightPos );

	shader.setUniform ( "Kd", 0.9f, 0.5f, 0.3f );
	shader.setUniform ( "Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f );
	shader.setUniform ( "Ks", 0.f, 0.f, 0.f );
	shader.setUniform ( "Shininess", 180.f );

	float dist = 0.f;

	for ( int i = 0 ; i < 4 ; i ++ )
	{
		model = mat4 ( 1.0f );
		model *= glm::translate ( vec3 ( dist * 0.6f - 1.f, 0.f, -dist ));
		model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

		setMatrices ();
		teapot->Render ();

		dist += 7.f;
	}

	shader.setUniform ( "Kd", 0.7f, 0.7f, 0.7f );
	shader.setUniform ( "Ka", 0.2f, 0.2f, 0.2f );
	shader.setUniform ( "Ks", 0.f, 0.f, 0.f );
	shader.setUniform ( "Shininess", 180.f );

	model = mat4 ( 1.f );

	setMatrices ();
	plane->Render ();
}


void SceneFog::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}