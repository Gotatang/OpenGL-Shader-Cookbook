#include "sceneads.h"


#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

#include <iostream>

using std::cerr;
using std::endl;

SceneADS::SceneADS () : angle ( 0.f )
{}


SceneADS::~SceneADS ()
{
	delete torus;
}

void SceneADS::Init ()
{
	try
	{
		adsShader.CompileShader ( "phong.vert" );
		adsShader.CompileShader ( "phong.frag" );
		adsShader.Link ();
		adsShader.Validate ();
		adsShader.Use ();
	}

	catch ( GLSLProgramException &e )
	{
		cerr << e.what () << endl;
		exit ( EXIT_FAILURE );
	}

	glEnable ( GL_DEPTH_TEST );


	torus = new VBOTorus ( 0.7f, 0.3f, 3690, 3690 );

	model = mat4 ( 1.f );
	model = glm::rotate ( glm::radians ( -35.f ), vec3 ( 1.f, 0.f, 0.f ));
	model = glm::rotate ( glm::radians ( 35.f ), vec3 ( 0.f, 1.f, 0.f ));

	view = glm::lookAt ( vec3 ( 0.f, 0.f, 2.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 70.f ), ( float ) 800 / 600, 0.3f, 100.f );

	vec4 worldLight = vec4 ( 5.f, 5.f, 2.f, 1.f );

	adsShader.setUniform ( "Material.Kd", 0.9f, 0.5, 0.3f );
	adsShader.setUniform ( "Light.Ld", 1.f, 1.f, 1.f );
	adsShader.setUniform ( "Light.Position", view * worldLight );
	adsShader.setUniform ( "Material.Ka", 0.9f, 0.5f, 0.3f );
	adsShader.setUniform ( "Light.La", .4f, .4f, .4f );
	adsShader.setUniform ( "Material.Ks", 0.8f, 0.8f, 0.8f );
	adsShader.setUniform ( "Light.Ls", 1.f, 1.f, 1.f );
	adsShader.setUniform ( "Material.Shininess", 100.f );
}

void SceneADS::Update ( float t )
{}

void SceneADS::Render ()
{
	model = mat4 ( 1.f );
	model = glm::rotate ( glm::radians ( angle ), vec3 ( 0.f, 1.f, 0.f ));
	model = glm::rotate ( glm::radians ( -35.f ), vec3 ( 1.f, 0.f, 0.f ));
	model = glm::rotate ( glm::radians ( 35.f ), vec3 ( 0.f, 1.f, 0.f ));

	setMatrices ();
	torus->Render ();
}


void SceneADS::setMatrices ()
{
	mat4 mv = view * model;

	adsShader.setUniform ( "ModelViewMatrix", mv );
	adsShader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	adsShader.setUniform ( "MVP", projection * mv );
}