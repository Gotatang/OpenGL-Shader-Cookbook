#ifndef __SCENEMULTILIGHT_H_
#define __SCENEMULTILIGHT_H_

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbomesh.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneMultiLight : public Scene
{

public:

	SceneMultiLight ();
	~SceneMultiLight ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOMesh* mesh;

	mat4 view;
	mat4 model;
	mat4 projection;

	void setMatrices ();
};

#endif