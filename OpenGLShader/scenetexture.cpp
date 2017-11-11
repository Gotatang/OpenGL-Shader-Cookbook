#include "scenetexture.h"

#include "tgaio.h"

#include <iostream>
using std::cerr;
using std::endl;

#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

SceneTexture::SceneTexture () : angle ( 0.f )
{}

SceneTexture::~SceneTexture ()
{
	delete plane;
	delete cube;
}


void SceneTexture::Init ()
{
	try
	{
		shader.CompileShader ( "texture.vert" );
		shader.CompileShader ( "texture.frag" );
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

	view = glm::lookAt ( vec3 ( 1.f, 1.25f, 1.25f ), vec3 ( 0.f, 0.f, 0.f ), vec3 ( 0.f, 1.f, 0.f ));

	projection = mat4 ( 1.f );
	projection = glm::perspective ( glm::radians ( 60.f ), ( float ) 800 / 600, 0.3f, 100.f );

	shader.setUniform ( "Light.intensity", vec3 ( 1.f, 1.f, 1.f ));

	// Load texture file
	GLint w, h;

	glActiveTexture ( GL_TEXTURE0 );

	GLubyte* data = TGAIO::read ( "../Media/Textures/brick1.tga", w, h );
	GLuint textureID;

	glGenTextures ( 1, &textureID );

	glBindTexture ( GL_TEXTURE_2D, textureID );
	glTexStorage2D ( GL_TEXTURE_2D, 1, GL_RGBA8, w, h );
	glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	delete [] data;

	// Set the Tex1 sampler uniform to refer to texture unit 0 or use layout ( binding = 0 ) uniform sampler2D Tex1;
	int location = glGetUniformLocation ( shader.getHandle (), "Tex1" );

	if ( location >= 0 )
	{
		glUniform1i ( location, 0 );
	}
}

void SceneTexture::Update ( float t )
{}

void SceneTexture::Render ()
{
	shader.setUniform ( "Light.position", vec4 ( 0.f, 0.f, 0.f, 1.f ));
	shader.setUniform ( "Material.Kd", 0.9f, 0.9f, 0.9f );
	shader.setUniform ( "Materail.Ks", 0.95f, 0.95f, 0.95f );
	shader.setUniform ( "Material.Ka", 0.1f, 0.1f, 0.1f );
	shader.setUniform ( "Material.Shininess", 100.f );

	model = mat4 ( 1.f );

	setMatrices ();
	cube->Render ();
}


void SceneTexture::setMatrices ()
{
	mat4 mv = view * model;

	shader.setUniform ( "ModelViewMatrix", mv );
	shader.setUniform ( "NormalMatrix", mat3 ( vec3 ( mv [ 0 ]), vec3 ( mv [ 1 ]), vec3 ( mv [ 2 ])));
	shader.setUniform ( "MVP", projection * mv );
}