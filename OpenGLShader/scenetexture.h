#ifndef __SCENETEXTURE_H_
#define __SCENETEXTURE_H_

#include "GLBasics.h"

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneTexture : public Scene
{

public:

	SceneTexture ();
	~SceneTexture ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOCube* cube;

	mat4 model;
	mat4 view;
	mat4 projection;

	float angle;

	void setMatrices ();
};

#endif