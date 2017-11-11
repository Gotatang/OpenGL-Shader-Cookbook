#include "scenecartoon.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"


SceneCartoon::SceneCartoon () : fPrev ( 0.f )
{}

SceneCartoon::~SceneCartoon ()
{
	delete plane;
	delete teapot;
	delete torus;
}


void SceneCartoon::Init ()
{
	try
	{
		shader.CompileShader ( "cartoon.vert" );
		shader.CompileShader ( "cartoon.frag" );
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
	teapot = new VBOTeapot ( 14, mat4  ( 1.f ));
	torus = new VBOTorus ( 1.75f * 0.75f, 0.75f * 0.75f, 50, 50 );

	view = glm::lookAt ( vec3 ( 4.f, 4.f, 6.5f ), vec3 ( 0.f, 0.75f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	angle = 0.f;

	shader.setUniform ( "Light.intensity", vec3 ( 0.9f, 0.9f, 0.9f ));
}

void SceneCartoon::Update ( float t )
{
	float deltaT = t - fPrev;

	if ( fPrev == 0.f )
	{
		deltaT = 0.f;
	}

	fPrev = t;

	angle += 0.25f * deltaT;

	if ( angle > glm::two_pi <float> ())
	{
		angle -= glm::two_pi <float> ();
	}
}

void SceneCartoon::Render ()
{
	vec4 lightPos = vec4 ( 10.f * cos ( angle ), 10.f, 10.f * sin ( angle ), 1.0 );
	shader .setUniform ( "Light.position", view * lightPos );

	shader.setUniform ( "Kd", 0.9f, 0.5f, 0.3f );
	shader.setUniform ( "Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, 0.f, -2.f ));
	model *= glm::rotate ( glm::radians ( 45.f ), vec3 ( 0.f, 1.f, 0.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();


	shader.setUniform ( "Kd", 0.9f, 0.5f, 0.3f );
	shader.setUniform ( "Ka", 0.9f * 0.3f, 0.5f * 0.3f, 0.3f * 0.3f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( -1.f, 0.75f, 3.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));
	
	setMatrices ();
	torus->Render ();


	shader.setUniform ( "Kd", 0.7f, 0.7f, 0.7f );
	shader.setUniform ( "Ka", 0.2f, 0.2f,  0.2f );


	model = mat4 ( 1.f );
	
	setMatrices ();
	plane->Render ();
}


void SceneCartoon::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}