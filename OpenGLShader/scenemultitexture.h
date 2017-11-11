#ifndef __SCENEMULTITEXTURE_H_
#define __SCENEMULTITEXTURE_H_

#include "scene.h"
#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneMultiTexture : public Scene
{

public:

	SceneMultiTexture ();
	~SceneMultiTexture ();

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