#include "sceneprojectedtexture.h"

#include "tgaio.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\constants.hpp"

SceneProjectedTexture::SceneProjectedTexture () : angle ( 0.f ), fPrev ( 0.f ), rotSpeed ( glm::two_pi <float> () / 8 )
{}

SceneProjectedTexture::~SceneProjectedTexture ()
{
	delete plane;
	delete teapot;
	delete sky;
}


void SceneProjectedTexture::Init ()
{
	try
	{
		shader.CompileShader ( "projectedtexture.vert" );
		shader.CompileShader ( "projectedtexture.frag" );
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

	teapot = new VBOTeapot ( 14, mat4 ( 1.f ));
	plane = new VBOPlane ( 100.f, 100.f, 1, 1 );

	projection = mat4 ( 1.0f );
	projection = glm::perspective ( glm::radians ( 50.f ), ( float ) 800 / 600, 0.3f, 1000.f );

	angle = glm::radians ( 90.f );

	// Proj settings
	vec3 projPos = vec3 ( 2.f, 5.f, 5.f );
	vec3 projAt = vec3 ( -2.f, -4.f, 0.f );
	vec3 projUp = vec3 ( 0.f, 1.f, 0.f );
	mat4 projView = glm::lookAt ( projPos, projAt, projUp );
	mat4 projProj = glm::perspective ( glm::radians ( 30.f ), 1.f, 0.2f, 1000.f );
	mat4 projScaleTrans = glm::translate ( vec3 ( 0.5f )) * glm::scale ( vec3 ( 0.5f ));
	shader.setUniform ( "ProjectorMatrix", projScaleTrans * projProj * projView );

	// Load textures
	glActiveTexture ( GL_TEXTURE0 );
	TGAIO::loadTexture ( "../Media/Textures/flower.tga" );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );

	shader.setUniform ( "Light.Position", vec4 ( 0.f, 0.f, 0.f, 1.f ));
	shader.setUniform ( "Light.Intensity", vec3 ( 1.f, 1.f, 1.f ));
}

void SceneProjectedTexture::Update ( float t )
{
	float deltaT = t - fPrev;

	if ( fPrev == 0.f )
	{
		deltaT = 0.f;
	}

	fPrev = t;

	angle += rotSpeed * deltaT;

	if ( angle > glm::two_pi <float> ())
	{
		angle -= glm::two_pi <float> ();
	}
}

void SceneProjectedTexture::Render ()
{
	vec3 cameraPos = vec3 ( 7.f * cos ( angle ), 2.f, 7.f * sin ( angle ));
	view = glm::lookAt ( cameraPos, vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	shader.setUniform ( "Material.Kd", 0.5f, 0.2f, 0.1f );
	shader.setUniform ( "Material.Ks", 0.95f, 0.95f, 0.95f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 100.f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -1.f, 0.f ));
	model *= glm::rotate ( glm::radians ( - 90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();

	shader.setUniform ( "Material.Kd", 0.4f, 0.4f, 0.4f );
	shader.setUniform ( "Material.Ks", 0.f, 0.f, 0.f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 1.f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -0.75f, 0.f ));

	setMatrices ();
	plane->Render ();
}


void SceneProjectedTexture::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelMatrix", model );
	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}