#ifndef __SCENEDIRECTIONAL_H_
#define __SCENEDIRECTIONAL_Hç

#include "scene.h"
#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbotorus.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneDirectional : public Scene
{

public:

	SceneDirectional ();
	~SceneDirectional ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOTorus* torus;

	mat4 view;
	mat4 model;
	mat4 projection;

	void setMatrices ();
};

#endif