#ifndef __SCENEALPHAMAP_H_
#define __SCENEALPHAMAP_H_

#include "GLBasics.h"

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboteapot.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneAlphaMap : public Scene
{

public:

	SceneAlphaMap ();
	~SceneAlphaMap ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOCube* cube;
	VBOTeapot* teapot;

	mat4 model;
	mat4 view;
	mat4 projection;

	float angle, fPrev, rotSpeed;

	void setMatrices ();
};

#endif