#include "sceneflat.h"

#include <iostream>

using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

using glm::vec3;

SceneFlat::SceneFlat () : angle ( 0.f )
{}

SceneFlat::~SceneFlat ()
{
	delete teapot;
}

void SceneFlat::Init ()
{
	try
	{
		shader.CompileShader ( "flat.vert" );
		shader.CompileShader ( "flat.frag" );
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

	mat4 transform = glm::translate ( mat4 ( 1.f ), vec3 ( 0.f, 1.5f, 0.25f ));
	teapot = new VBOTeapot ( 500, transform );

	view = glm::lookAt ( vec3 ( 2.0f, 4.0f, 2.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians(70.0f), (float) 800 / 600, 0.3f, 100.0f );

	shader.setUniform ("Material.Kd", 0.9f, 0.5f, 0.3f);
    shader.setUniform ("Light.Ld", 1.0f, 1.0f, 1.0f);
    shader.setUniform ("Material.Ka", 0.9f, 0.5f, 0.3f);
    shader.setUniform ("Light.La", 0.4f, 0.4f, 0.4f);
    shader.setUniform ("Material.Ks", 0.8f, 0.8f, 0.8f);
    shader.setUniform ("Light.Ls", 1.0f, 1.0f, 1.0f);
    shader.setUniform ("Material.Shininess", 100.0f);
}

void SceneFlat::Update ( float t )
{}

void SceneFlat::Render ()
{
	vec4 worldLight = vec4 ( 2.f, 4.f, 2.f, 1.f );
	model = glm::rotate ( glm::radians ( angle ), vec3 ( 0.f, 1.f, 0.f ));
	shader.setUniform("Light.Position", view * model * worldLight );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -1.f, 0.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	angle ++;

	setMatrices ();
	teapot->Render ();
}

void SceneFlat::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}