#include "sceneperfragment.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

ScenePerFragment::ScenePerFragment () : fPrev ( 0 )
{}


ScenePerFragment::~ScenePerFragment ()
{
	delete plane;
	delete teapot;
	delete torus;
}


void ScenePerFragment::Init ()
{
	try
	{
		shader.CompileShader ( "perfragment.vert" );
		shader.CompileShader ( "perfragment.frag" );
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
	torus = new VBOTorus ( 0.7f * 2, 0.3f * 2, 50, 50 );

	view = glm::lookAt ( vec3 ( 0.f, 3.f, 5.f ), vec3 ( 0.f, 0.75f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	angle = 0.957283f;

	shader.setUniform ( "LightIntensity", vec3 ( 0.9f, 0.9f, 0.9f ));
}

void ScenePerFragment::Update ( float t )
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

void ScenePerFragment::Render ()
{
	shader.setUniform ( "LightPosition", view * vec4 ( 10.f * cos ( angle ), 3.f, 10.f * sin ( angle ), 1.f ));
	shader.setUniform ( "Kd", 0.9f, 0.5f, 0.3f );
	shader.setUniform ( "Ks", 0.95f, 0.95f, 0.95f );
	shader.setUniform ( "Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Shininess", 100.f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, 0.f, 0.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();


	shader.setUniform ( "Kd", 0.7f, 0.7f, 0.7f );
	shader.setUniform ( "Ks", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Shininess", 180.f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -0.45f, 0.f ));

	setMatrices ();
	plane->Render ();
}


void ScenePerFragment::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}