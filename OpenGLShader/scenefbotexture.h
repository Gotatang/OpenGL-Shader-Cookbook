#ifndef __SCENEFBOTEXTURE_H_
#define __SCENEFBOTEXTURE_H_

#include "scene.h"
#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

#include "glm\glm.hpp"
using glm::mat4;


class SceneFBOTexture : public Scene
{

public:

	SceneFBOTexture ();
	~SceneFBOTexture ();

	void Init ();
	void Update ( float t );
	void Render ();


private:

	GLSLProgram shader;

	GLuint fboHandle;
	VBOPlane *plane;
	VBOCube *cube;
	VBOTeapot *teapot;

	mat4 model;
	mat4 view;
	mat4 projection;
	float angle, fprev, rotSpeed;

	void setMatrices ();
	void setupFBO ();
	void renderToTexture ();
	void renderScene ();
};

#endif