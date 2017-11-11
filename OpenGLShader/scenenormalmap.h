#ifndef __SCENENORMALMAP_H_
#define __SCENENORMALMAP_H_

#include "GLBasics.h"

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbomesh.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneNormalMap : public Scene
{

public:

	SceneNormalMap ();
	~SceneNormalMap ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOMesh* ogre;

	mat4 model;
	mat4 view;
	mat4 projection;

	float angle, fPrev, rotSpeed;

	void setMatrices ();
};

#endif