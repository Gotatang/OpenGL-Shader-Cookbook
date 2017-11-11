#ifndef __SCENETWOSIDE_H_
#define __SCENETWOSIDE_H_

#include "scene.h"

#include "GLBasics.h"
#include "GLSLProgram.h"
#include "vboteapot.h"

#include "glm\glm.hpp"

using glm::mat4;

class SceneTwoSide : public Scene
{

public:

	SceneTwoSide ();
	~SceneTwoSide ();

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