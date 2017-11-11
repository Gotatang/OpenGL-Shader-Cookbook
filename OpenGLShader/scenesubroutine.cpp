#include "scenesubroutine.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
using glm::mat3;

using glm::vec3;
using glm::vec4;


#include <iostream>
using std::cerr;
using std::endl;

SceneSubroutine::SceneSubroutine () : angle ( 0.f )
{}

SceneSubroutine::~SceneSubroutine ()
{
	delete torus;
	delete teapot;
}


void SceneSubroutine::Init ()
{
	try
	{
		shader.CompileShader ( "subroutine.vert" );
		shader.CompileShader ( "subroutine.frag" );
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

	teapot = new VBOTeapot ( 500, mat4 ( 1.f ));
	torus = new VBOTorus ( 0.7f, 0.3f, 30, 30 );

	view = glm::lookAt ( vec3 ( 0.f, 0.f, 10.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

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

void SceneSubroutine::Update ( float t )
{}

void SceneSubroutine::Render ()
{
	vec4 lightPos = vec4 ( 0.f, 0.f, 0.f, 1.f );
	shader.setUniform ( "Light.Position", lightPos );

	GLuint programmHandle = shader.getHandle ();
	GLuint adsIndex =  glGetSubroutineIndex ( programmHandle, GL_VERTEX_SHADER, "phongModel" );
	GLuint diffuseIndex = glGetSubroutineIndex ( programmHandle, GL_VERTEX_SHADER, "diffuseOnly" );

	glUniformSubroutinesuiv ( GL_VERTEX_SHADER, 1, &adsIndex );
	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( -3.f, -1.5f, 0.f ));
	model *= glm::rotate ( glm::radians ( - 90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();

	glUniformSubroutinesuiv ( GL_VERTEX_SHADER, 1, &diffuseIndex );
	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 3.f, -1.5f, 0.f ));
	model *= glm::rotate ( glm::radians ( - 90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();
}


void SceneSubroutine::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}