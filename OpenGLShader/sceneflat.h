#ifndef __SCENEFLAT_H_
#define __SCENEFLAT_H_

#include "GLBasics.h"
#include "GLSLProgram.h"

#include "scene.h"

#include "vboteapot.h"

#include "glm\glm.hpp"

using glm::mat4;

class SceneFlat : public Scene
{

public:

	SceneFlat ();
	~SceneFlat ();

	void Init ();
	void Update ( float t );
	void Render ();

private :

	GLSLProgram shader;

	VBOTeapot* teapot;
	float angle;

	mat4 model;
	mat4 view;
	mat4 projection;

	void setMatrices ();
};

#endif