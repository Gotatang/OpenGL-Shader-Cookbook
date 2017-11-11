#ifndef __SCENESUBROUTINE_H_
#define __SCENESUBROUTINE_H_

#include "GLBasics.h"
#include "GLSLProgram.h"

#include "glm\glm.hpp"

using glm::mat4;


#include "scene.h"

#include "vboTorus.h"
#include "vboteapot.h"


class SceneSubroutine : public Scene
{

public:

	SceneSubroutine ();
	~SceneSubroutine ();

	void Init ();
	void Update ( float t );
	void Render ();

private :

	GLSLProgram shader;

	VBOTorus* torus;
	VBOTeapot* teapot;
	float angle;

	mat4 model;
	mat4 view;
	mat4 projection;

	void setMatrices ();
};

#endif