#include "scenealphamap.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\constants.hpp"

#include "tgaio.h"


SceneAlphaMap::SceneAlphaMap () : angle ( 0.f ), fPrev ( 0.f ), rotSpeed ( glm::two_pi <float> () / 50.f )
{}

SceneAlphaMap::~SceneAlphaMap ()
{
	delete plane;
	delete cube;
	delete teapot;
}


void SceneAlphaMap::Init ()
{
	try
	{
		shader.CompileShader ( "alphamap.vert" );
		shader.CompileShader ( "alphamap.frag" );
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
	teapot = new VBOTeapot ( 500, mat4 ( 1.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	shader.setUniform ( "Light.intensity", 1.f, 1.f, 1.f );

	// Load cement texture file
	glActiveTexture ( GL_TEXTURE0 );
	TGAIO::loadTexture ( "../Media/Textures/cement.tga" );

	// Load moss texture fie
	glActiveTexture ( GL_TEXTURE1 );
	TGAIO::loadTexture ( "../Media/Textures/moss.tga" );
}

void SceneAlphaMap::Update ( float t )
{
	float deltaT = t - fPrev;

	if ( fPrev == 0.f )
	{
		deltaT = 0.f;
	}

	fPrev = t;

	angle += rotSpeed * deltaT;

	if ( this->bAnimate )
	{
		if ( angle > glm::two_pi <float> ())
		{
			angle -=  glm::two_pi <float> ();
		} 
	}

}

void SceneAlphaMap::Render ()
{
	vec3 cameraPos = vec3 ( 6.f * cos ( angle ), 0.25f, 6.f * sin ( angle ));
	view = glm::lookAt ( cameraPos, vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	shader.setUniform ( "Light.position", vec4 ( 0.f, 0.f, 0.f, 1.f ));
	shader.setUniform ( "Material.Kd", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Materail.Ka", 0.f, 0.f, 0.f );
	shader.setUniform ( "Material.Ks", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Materail.shininess", 100.f );

	model = mat4 ( 1.f );
	model = glm::translate ( model, vec3 ( 0.f, -1.5f, 0.f ));
	model = glm::rotate ( model, glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));
	//model = glm::rotate ( model, glm::radians ( 90.f ), vec3 ( 0.f, 0.f, 1.f ));

	setMatrices ();
	teapot->Render ();
}


void SceneAlphaMap::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}