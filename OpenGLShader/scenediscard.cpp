#include "scenediscard.h"


#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
using glm::vec3;

#include <iostream>
using std::cerr;
using std::endl;

SceneDiscard::SceneDiscard () : angle ( 0.f )
{}

SceneDiscard::~SceneDiscard ()
{
	delete teapot;
}


void SceneDiscard::Init ()
{
	try
	{
		shader.CompileShader ( "discard.vert" );
		shader.CompileShader ( "discard.frag" );
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

	teapot = new VBOTeapot ( 13, mat4  ( 1.f ));
	
	view = glm::lookAt ( vec3 ( 0.f, 0.f, 7.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 50.f ), ( float ) 800 / 600, 0.3f, 100.f );

	shader.setUniform ("Material.Kd", 0.9f, 0.5f, 0.3f);
    shader.setUniform ("Light.Ld", 1.0f, 1.0f, 1.0f);
    shader.setUniform ("Material.Ka", 0.9f, 0.5f, 0.3f);
    shader.setUniform ("Light.La", 0.4f, 0.4f, 0.4f);
    shader.setUniform ("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader.setUniform ("Light.Ls", 1.0f, 1.0f, 1.0f);
    shader.setUniform ("Material.Shininess", 100.0f);
}

void SceneDiscard::Update ( float t )
{}

void SceneDiscard::Render ()
{
	vec4 lightPos = vec4 ( 0.f, 0.f, 0.f, 1.f );
	shader.setUniform ( "Light.Position", lightPos );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -1.5f, 0.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();
}


void SceneDiscard::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}