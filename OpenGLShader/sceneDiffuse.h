#ifndef __DIFFUSE_H_
#define __DIFFUSE_H_

#include "scene.h"
#include "vboTorus.h"
#include "GLSLProgram.h"

#include "glm\glm.hpp"

using glm::mat4;

class SceneDiffuse : public Scene
{

public:

	SceneDiffuse();
	~SceneDiffuse();

	void Init ();
	void Update ( float t );
	void Render ();

	void setMatrices ();

private:

	mat4 model;
	mat4 view;
	mat4 projection;

	VBOTorus* firstTorus;
	GLSLProgram diffuseShader;
};

#endif