#include "sceneDiffuse.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

using std::cerr;


#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

using glm::vec3;


SceneDiffuse::SceneDiffuse()
{}

SceneDiffuse::~SceneDiffuse ()
{
	delete firstTorus;
}

void SceneDiffuse::Init ()
{
	try
	{
		diffuseShader.CompileShader ( "diffuse.vert" );
		diffuseShader.CompileShader ( "diffuse.frag" );
		diffuseShader.Link ();
		diffuseShader.Validate ();
		diffuseShader.Use ();
	}

	catch ( GLSLProgramException &e )
	{
		std::cerr << e.what () << std::endl;
	}

	glEnable ( GL_DEPTH_TEST );

	diffuseShader.printfActiveAttribs ();
	diffuseShader.printfActiveUniforms ();
	diffuseShader.printfActiveUniformBlocks ();

	firstTorus = new VBOTorus ( .7f, .3f, 3690, 3690 ); // Max 3690 & 3690

	model = mat4 ( 1.0f );
	model *= glm::rotate ( glm::radians ( -35.f ), vec3 ( 1.0f, 0.0f, 0.0f ));
	model *= glm::rotate ( glm::radians ( 35.f ), vec3 ( 0.0f, 1.0f, 0.0f ));

	view = glm::lookAt ( vec3 ( 0.0f, 0.0f, 2.0f ), vec3 ( 0.0f, 0.0f, 0.0f ), vec3 ( 0.0f, 1.0f, 0.0f ));

	projection = mat4 ( 1.0f );

	diffuseShader.setUniform ( "Kd", 0.9f, 0.5f, 0.3f );
	diffuseShader.setUniform ( "Ld", 1.0f, 1.0f, 1.0f );
	diffuseShader.setUniform ( "LightPosition", view * vec4 ( 5.0f, 5.0f, 2.0f, 1.0f ));
}

void SceneDiffuse::Update ( float t )
{
	     projection = glm::perspective(glm::radians(70.0f), (float) 800 / 600, 0.3f, 100.0f);
}

void SceneDiffuse::Render ()
{
	setMatrices ();
	firstTorus->Render ();
}


void SceneDiffuse::setMatrices ()
{
	mat4 modelView = view * model;

	diffuseShader.setUniform ( "ModelViewMatrix", modelView );
	diffuseShader.setUniform ( "NormalMatrix", mat3 ( vec3 ( modelView [ 0 ]), vec3 ( modelView [ 1 ]), vec3 ( modelView [ 2 ])));
	diffuseShader.setUniform ( "MVP", projection * modelView );
}