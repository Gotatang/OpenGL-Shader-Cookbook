#include "scenenormalmap.h"

#include "tgaio.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\constants.hpp"

SceneNormalMap::SceneNormalMap () : angle ( 0.f ), fPrev ( 0.f ), rotSpeed ( glm::two_pi <float> () / 8.f )
{}

SceneNormalMap::~SceneNormalMap ()
{
	delete ogre;
}


void SceneNormalMap::Init ()
{
	try
	{
		shader.CompileShader ( "normalmap.vert" );
		shader.CompileShader ( "normalmap.frag" );
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

	ogre = new VBOMesh ( "../Media/Model/bs_ears.obj", false, true, true );

	view = glm::lookAt ( vec3 ( -1.f, 0.25f, 2.f ), vec3  ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::ortho ( -0.4f * 2.f, 0.4f * 2.f, -0.3f * 2.f, 0.3f * 2.f, 0.1f, 100.f );

	angle = glm::radians ( 100.f );

	shader.setUniform ( "Light.intensity", vec3 ( 0.9f, 0.9f, 0.9f ));

	// Load diffuse texture
	glActiveTexture ( GL_TEXTURE0 );
	TGAIO::loadTexture ( "../Media/Textures/ogre_diffuse.tga" );

	// Load normal map
	glActiveTexture ( GL_TEXTURE1 );
	TGAIO::loadTexture ( "../Media/Textures/ogre_normalmap.tga" );
}

void SceneNormalMap::Update ( float t )
{
	float deltaT = t - fPrev;

	if ( fPrev == 0.f )
	{
		deltaT = 0.f;
	}

	fPrev = t;

	if ( this->bAnimate )
	{
		angle += rotSpeed * deltaT;

		if ( angle > glm::two_pi <float> ())
		{
			angle -= glm::two_pi <float> ();
		}
	}
}

void SceneNormalMap::Render ()
{
	shader.setUniform ( "Light.position", view * vec4 ( 10.f * cos ( angle ), 1.f, 10.f * sin ( angle ), 1.f ));
	shader.setUniform ( "Material.Ks", 0.2f, 0.2f, 0.2f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.shininess", 1.f );

	model = mat4 ( 1.f );

	setMatrices ();
	ogre->Render ();
}


void SceneNormalMap::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}