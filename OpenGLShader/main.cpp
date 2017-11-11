#include "gl_core_4_4.h"

#include "GLFW\glfw3.h"
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <typeinfo>
#include "GLUtils.h"

// Chapter 2
#include "sceneDiffuse.h"
#include "sceneads.h"
#include "scenetwoside.h"
#include "sceneflat.h"
#include "scenesubroutine.h"
#include "scenediscard.h"

// Chapter 3
#include "scenemultilight.h"
#include "scenedirectional.h"
#include "sceneperfragment.h"
#include "scenespotlight.h"
#include "scenecartoon.h"
#include "scenefog.h"

// Chapter 4
#include "scenetexture.h"
#include "scenemultitexture.h"
#include "scenealphamap.h"
#include "scenenormalmap.h"
#include "scenereflectcube.h"
#include "scenerefractcube.h"
#include "sceneprojectedtexture.h"
#include "scenefbotexture.h"
#include "scenesamplerobj.h"

// Chapter 7
#include "sceneshadowmap.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


GLFWwindow* window = NULL;

#define SHADOWMAP// ADS, DIFFUSE, TWOSIDE, FLAT, SUBROUTINE, DISCARD, MULTILIGHT, DIRECTIONAL, PERFRAGMENT, SPOTLIGHT, CARTOON, FOG, TEXTURE, MULTITEXTURE, APLHAMAP, NORMALMAP, REFLECTCUBE, REFRACTCUBE, PROJTEX, TEXTUREFBO, SAMPLER, SHADOWMAP


void UpdateFPSCounter ( GLFWwindow* _window )
{
	static double previousSeconds = glfwGetTime ();
	static int frameCount;
	double currentSeconds = glfwGetTime ();
	double elapsedTime = currentSeconds - previousSeconds;

	if ( elapsedTime > 0.25 )
	{
		previousSeconds = currentSeconds;

		double fps = ( double ) frameCount / elapsedTime;
		char buffer [ 128 ];

		sprintf ( buffer, "fps : %.2f", fps );
		glfwSetWindowTitle ( _window, buffer );

		frameCount = 0;
	}

	frameCount ++;
}

void framebuffer_size_callback ( GLFWwindow* _window, int _width, int _height )
{
	glViewport ( 0, 0, _width, _height );
}

int main (int argc, char* argv[])
{
	if (!glfwInit())
		exit ( EXIT_FAILURE );

	glfwWindowHint ( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint ( GLFW_CONTEXT_VERSION_MINOR, 4 );
	glfwWindowHint ( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint ( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint ( GLFW_RESIZABLE, GL_FALSE );
	glfwWindowHint ( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );

	window = glfwCreateWindow ( WINDOW_WIDTH, WINDOW_HEIGHT, "Shader", NULL, NULL );

	if ( !window )
	{
		glfwTerminate ();
		exit ( EXIT_FAILURE );
	}

	glfwMakeContextCurrent ( window );

	int loaded = ogl_LoadFunctions ();

	if ( loaded == ogl_LOAD_FAILED ) // Charge les fonctions opengl 4.4
	{
		glfwDestroyWindow ( window );
		exit ( EXIT_FAILURE );
	}

	printf ( "%d functions failed to load\n", loaded - ogl_LOAD_SUCCEEDED ); // Nombre de fonction qui n'ont pas était charger

	glfwSetWindowSizeCallback ( window, framebuffer_size_callback );
	
	glClearColor ( 0.5f, 0.5f, 0.5f, 1.f );

	glDebugMessageCallback ( GLUtils::debugCallBack, NULL );
	glDebugMessageControl ( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE );

	glViewport ( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

	GLUtils::dumpGLInfo ();

#ifdef DIFFUSE
	SceneDiffuse* pScene = new SceneDiffuse;
#endif
#ifdef ADS
	SceneADS* pScene = new SceneADS;
#endif
#ifdef TWOSIDE
	SceneTwoSide* pScene = new SceneTwoSide;
#endif
#ifdef FLAT
	SceneFlat* pScene = new SceneFlat;
#endif
#ifdef SUBROUTINE
	SceneSubroutine* pScene = new SceneSubroutine;
#endif
#ifdef DISCARD
	SceneDiscard* pScene = new SceneDiscard;
#endif
#ifdef MULTILIGHT
	SceneMultiLight* pScene = new SceneMultiLight;
#endif
#ifdef DIRECTIONAL
	SceneDirectional* pScene = new SceneDirectional;
#endif
#ifdef PERFRAGMENT
	ScenePerFragment* pScene = new ScenePerFragment;
#endif
#ifdef SPOTLIGHT
	SceneSpotlight* pScene = new SceneSpotlight;
#endif
#ifdef CARTOON
	SceneCartoon* pScene = new SceneCartoon;
#endif
#ifdef FOG
	SceneFog* pScene = new SceneFog;
#endif
#ifdef TEXTURE
	SceneTexture* pScene = new SceneTexture;
#endif
#ifdef MULTITEXTURE
	SceneMultiTexture* pScene = new SceneMultiTexture;
#endif
#ifdef APLHAMAP
	SceneAlphaMap* pScene = new SceneAlphaMap;
#endif
#ifdef NORMALMAP
	SceneNormalMap* pScene = new SceneNormalMap;
#endif
#ifdef REFLECTCUBE
	SceneReflectCube* pScene = new SceneReflectCube;
#endif
#ifdef REFRACTCUBE
	SceneRefractCube* pScene = new SceneRefractCube;
#endif
#ifdef PROJTEX
	SceneProjectedTexture* pScene = new SceneProjectedTexture;
#endif
#ifdef TEXTUREFBO
	SceneFBOTexture* pScene = new SceneFBOTexture;
#endif
#ifdef SAMPLER
	SceneSamplerObj* pScene = new SceneSamplerObj;
#endif
#ifdef SHADOWMAP
	SceneShadowMap* pScene = new SceneShadowMap;
#endif

	pScene->Init ();

	////////// Shader //////////

	/*float positionData[] =
	{
		-0.8f, -0.8f, 0.0f,
		0.8f, -0.8f, 0.0f,
		0.8f,  0.8f, 0.0f,
		-0.8f, -0.8f, 0.0f,
		0.8f, 0.8f, 0.0f,
		-0.8f, 0.8f, 0.0f
     };

    float tcData[] =
	{
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    GLuint VAOHandle;

	// Create and populate the buffer object
	GLuint VBOHandle [ 2 ];
	glGenBuffers ( 2, VBOHandle );

	GLuint positionBufferHandle = VBOHandle [ 0 ];
	GLuint colorBufferHandle = VBOHandle [ 1 ];


	// Populate the positon buffer
	glBindBuffer ( GL_ARRAY_BUFFER, positionBufferHandle );
	glBufferData ( GL_ARRAY_BUFFER, 18 * sizeof ( GLfloat ), positionData, GL_STATIC_DRAW );


	// Populate the color buffer
	glBindBuffer ( GL_ARRAY_BUFFER, colorBufferHandle );
	glBufferData ( GL_ARRAY_BUFFER, 12 * sizeof ( GLfloat ), tcData, GL_STATIC_DRAW );


	// Create and setup the vertex array object
	glGenVertexArrays ( 1, &VAOHandle );
	glBindVertexArray ( VAOHandle );


	// Enable the vertex attribute array
	glEnableVertexAttribArray ( 0 );		// Vertex position
	glEnableVertexAttribArray ( 1 );		// Vertex color


	// Map index 0 to the position buffer
	glBindBuffer ( GL_ARRAY_BUFFER, positionBufferHandle );
	glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


	// Map index 1 to the color buffer
	glBindBuffer ( GL_ARRAY_BUFFER, colorBufferHandle );
	glVertexAttribPointer ( 1, 2, GL_FLOAT, GL_FALSE, 0, NULL );

	//lEnable ( GL_BLEND );
	//glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );*/


	// Other way to do the last step

	/*	// Create and setup the vertex array object
		glGenVertexArrays ( 1, &VAOHandle );
		glBindVertexArray ( VAOHandle );


		// Enable the vertex attribute array
		glEnableVertexAttribArray ( 0 );		// Vertex position
		glEnableVertexAttribArray ( 1 );		// Vertex color

		glBindVertexBuffer ( 0, positionBufferHandle, 0, sizeof ( GLfloat ) * 3 );	Last argument -> stride between elements
		glBindVertexBuffer ( 1, colorBufferHandle, 0, sizeof ( GLfloat ) * 3 );		<- Part of the VAO state

		glVertexAttribFormat ( 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexAttribBinding ( 0, 0 );		First argument -> location in the vertex buffer

		glVertexAttribFormat ( 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexAttribBinding ( 1, 1 );		Second argument	-> index within the vertex buffer		*/


	double lastTime = glfwGetTime ();
	int nbFrames = 0;

	while ( !glfwWindowShouldClose ( window ))
	{
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glfwPollEvents ();

		UpdateFPSCounter ( window );
		pScene->Update (( float ) glfwGetTime ());
		pScene->Render ();

		glfwSwapBuffers ( window );
	}

	glfwDestroyWindow ( window );
	glfwTerminate ();

	return 0;
}