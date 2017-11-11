#include "scenereflectcube.h"

#include "tgaio.h"

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\constants.hpp"

#include <iostream>
using std::cerr;
using std::endl;

SceneReflectCube::SceneReflectCube () : angle ( 0.f ), fprev ( 0.f ), rotSpeed ( glm::two_pi <float> () / 8.f )
{}

SceneReflectCube::~SceneReflectCube ()
{
	delete plane;
	delete teapot;
	delete torus;
	delete sky;
}


void SceneReflectCube::Init ()
{
	try
	{
		shader.CompileShader ( "cubemap_reflect.vert" );
		shader.CompileShader ( "cubemap_reflect.frag" );
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
	sky = new SkyBox ();
	plane = new VBOPlane ( 1.0f, 1.0f, 1, 1 );
	float c = 3.5f;
	torus = new VBOTorus ( 0.7f * c, 0.3f * c, 50, 50 );

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 50.f ), ( float ) 800 / 600, 0.3f, 100.f );

	angle = glm::radians ( 90.f );

	loadCubeMap ( "../Media/Textures/cubemap_night/night" );
}

void SceneReflectCube::Update ( float t )
{
	float deltaT = t - fprev;

	if ( fprev == 0.f )
	{
		deltaT = 0.f;
	}

	fprev = t;

	angle += rotSpeed * deltaT;

	if ( angle > glm::two_pi <float> ())
	{
		angle -= glm::two_pi <float> ();
	}
}

void SceneReflectCube::Render ()
{
	vec3 cameraPos = vec3 ( 7.f * cos ( angle ), 2.f, 7.f * sin ( angle ));
	view = glm::lookAt ( cameraPos, vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));
	shader.setUniform ( "WorldCameraPosition", cameraPos );

	shader.setUniform ( "DrawSkyBox", true );
	model = mat4 ( 1.f );

	setMatrices ();
	sky->Render ();
	shader.setUniform ( "DrawSkyBox", false );

	shader.setUniform ( "MaterialColor", vec4 ( 0.5f, 0.5f, 0.5f, 1.f ));
	shader.setUniform ( "ReflectFactor", 0.85f );

	model = mat4 ( 1.f );
	model *= glm::translate ( vec3 ( 0.f, -1.f, 0.f ));
	model *= glm::rotate ( glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();
}


void SceneReflectCube::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "MVP", projection * mv );
}

void SceneReflectCube::loadCubeMap ( const char* _baseFileName )
{
	glActiveTexture ( GL_TEXTURE0 );

	GLuint texID;
	glGenTextures ( 1, &texID );
	glBindTexture ( GL_TEXTURE_CUBE_MAP, texID );

	const char* suffixes [] = { "posx", "negx", "posy", "negy", "posz", "negz" };

	GLuint targets [] =
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	GLint w, h;

	// Allocate the cube map texture
	glTexStorage2D ( GL_TEXTURE_CUBE_MAP, 1, GL_RGBA8, 256, 256 );

	// Load each cube-map face
	for ( int i = 0 ; i < 6 ; i ++ )
	{
		string texName = string ( _baseFileName ) + "_" + suffixes [ i ] + ".tga";
		GLubyte* data = TGAIO::read ( texName.c_str (), w, h );
		
		glTexSubImage2D ( targets [ i ], 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

		delete [] data;
	}

	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
}