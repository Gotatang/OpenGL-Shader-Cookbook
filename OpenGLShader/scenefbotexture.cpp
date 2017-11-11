#include "scenefbotexture.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\constants.hpp"


SceneFBOTexture::SceneFBOTexture () : angle ( 0.f ), fprev ( 0.f ), rotSpeed ( glm::two_pi <float> () / 8.f )
{}

SceneFBOTexture::~SceneFBOTexture ()
{
	delete plane;
	delete cube;
	delete teapot;
}


void SceneFBOTexture::Init ()
{
	try
	{
		shader.CompileShader ( "fbotexture.vert" );
		shader.CompileShader ( "fbotexture.frag" );
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
	cube = new VBOCube ();
	teapot = new VBOTeapot ( 14, mat4 ( 1.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	angle = glm::radians ( 140.f );

	shader.setUniform ( "Light.Intensity", vec3 ( 1.f, 1.f, 1.f ));
	setupFBO ();

	// One pixel white texture
	GLuint whiteTextureHandle;
	GLubyte whiteTexture [] = { 255, 255, 255, 255 };

	glActiveTexture ( GL_TEXTURE1 );
	glGenTextures ( 1, &whiteTextureHandle );
	glBindTexture ( GL_TEXTURE_2D, whiteTextureHandle );
	glTexStorage2D ( GL_TEXTURE_2D, 1, GL_RGBA8, 1, 1 );
	glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_INT, whiteTexture );
}

void SceneFBOTexture::Update ( float t )
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

void SceneFBOTexture::Render ()
{
	glBindFramebuffer ( GL_FRAMEBUFFER, fboHandle );
	renderToTexture ();

	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
	renderScene ();
}


void SceneFBOTexture::renderToTexture ()
{
	glViewport ( 0, 0, 512, 512 );				// Viewport for the texture

	// Use the "white" texture here
	shader.setUniform ( "Texture", 1 );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	view = glm::lookAt ( vec3 ( 0.f, 0.f, 7.f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));
	projection = glm::perspective ( glm::radians ( 60.f ), 1.f, 0.3f, 100.f );

	shader.setUniform ( "Light.Position", vec4 ( 0.f, 0.f, 0.f, 1.f ));
	shader.setUniform ( "Material.Kd", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Material.Ks", 0.95f, 0.95f, 0.95f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 100.f );

	model = mat4 ( 1.0f );
	model = glm::translate ( model, vec3 ( 0.f, -1.5f, 0.f ));
	model = glm::rotate ( model, glm::radians ( -90.f ), vec3 ( 1.f, 0.f, 0.f ));

	setMatrices ();
	teapot->Render ();
}

void SceneFBOTexture::renderScene ()
{
	glViewport ( 0, 0, 800, 600 );				// Viewport for the texture

	shader.setUniform ( "Texture", 0 );

	glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	vec3 cameraPos = vec3 ( 2.f * cos ( angle ), 1.5f, 2.f * sin ( angle ));
	view = glm::lookAt ( cameraPos, vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = glm::perspective ( glm::radians ( 45.f ), ( float ) 800 / 600, 0.3f, 100.f );
	

	shader.setUniform ( "Light.Position", vec4 ( 0.f, 0.f, 0.f, 1.f ));
	shader.setUniform ( "Material.Kd", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Material.Ks", 0.f, 0.f, 0.f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 1.f );

	model = mat4 ( 1.0f );
	setMatrices ();
	cube->Render ();
}


void SceneFBOTexture::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}

void SceneFBOTexture::setupFBO ()
{
	// Generate and bind the framebuffer
	glGenFramebuffers ( 1, &fboHandle );
	glBindFramebuffer ( GL_FRAMEBUFFER, fboHandle );

	// Create the texture object
	GLuint renderTexture;
	
	glGenTextures ( 1, &renderTexture );
	glActiveTexture ( GL_TEXTURE0 );		// Use the texture unit 0
	glBindTexture ( GL_TEXTURE_2D, renderTexture );

	glTexStorage2D ( GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512 );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Bind the texture to the fbo, to the color attachement 0	cf ( layout ( location = 0 ))
	glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0 );	// Last argument : mip map level 0 = single level


	// Create the depth buffer
	GLuint depthBuffer;

	// Create and bind renderbuffer object
	glGenRenderbuffers ( 1, &depthBuffer );
	glBindRenderbuffer ( GL_RENDERBUFFER, depthBuffer );

	// Allocating space for the renderbuffer
	glRenderbufferStorage ( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512 ); // GL_DEPTH_COMPONENT because we are using it as a depthbuffer

	// Bind the depth buffer to the fbo, to the only possible depth buffer ( one by framebuffer )
	glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer );

	// Set the target for the fragment shader outputs
	GLenum drawBuffers [] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers ( 1, drawBuffers );

	// Unbind the framebuffer
	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
}