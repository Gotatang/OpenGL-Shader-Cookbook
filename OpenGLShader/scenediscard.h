#ifndef __SCENEDISCARD_H_
#define __SCENEDISCARD_H_

#include "scene.h"

#include "GLSLProgram.h"
#include "vboteapot.h"

#include "glm\glm.hpp"

class SceneDiscard : public Scene
{

public:

	SceneDiscard ();
	~SceneDiscard ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOTeapot* teapot;

	float angle;
	mat4 view;
	mat4 model;
	mat4 projection;

	void setMatrices ();
};

#endif