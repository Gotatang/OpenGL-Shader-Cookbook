#include "scenedirectional.h"


#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

SceneDirectional::SceneDirectional ()
{}

SceneDirectional::~SceneDirectional ()
{
	delete plane;
	delete torus;
}


void SceneDirectional::Init ()
{
	try
	{
		shader.CompileShader ( "directional.vert" );
		shader.CompileShader ( "directional.frag" );
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
	torus = new VBOTorus ( 0.7f, 0.3f, 100, 100 );

	view = glm::lookAt ( vec3 ( 1., 1.f, 1.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 70.f ), ( float ) 800 / 600, 0.3f, 100.f );

	shader.setUniform ( "LightPosition", view * vec4 (  1.f, 0.f, 0.f, 0.f ));
	shader.setUniform ( "LightIntensity", vec3 ( 0.8f, 0.8f, 0.8f ));
}

void SceneDirectional::Update ( float t )
{}

void SceneDirectional::Render ()
{
	shader.setUniform ( "Kd", 0.8f, 0.8f, 0.8f );
	shader.setUniform ( "Ks", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Shininess", 180.f );

	model = mat4 ( 1.f );
	model *= glm::rotate ( glm::radians ( 90.f ), vec3 ( 0.f, 1.f, 0.f ));
	setMatrices ();
	torus->Render ();

	shader.setUniform ( "Kd", 0.4f, 0.4f, 0.4f );
	shader.setUniform ( "Ks", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Shininess", 180.f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -0.45f, 0.f ));
	setMatrices ();
	//plane->Render ();
}


void SceneDirectional::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}