#ifndef __SCENEADS_H_
#define __SCENEADS_H_


#include "scene.h"

#include "GLSLProgram.h"
#include "vboTorus.h"


#include "glm\glm.hpp"

using glm::mat4;

class SceneADS : public Scene
{

public:

	SceneADS ();
	~SceneADS ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	VBOTorus* torus;
	GLSLProgram adsShader;

	float angle;

	mat4 model;
	mat4 view;
	mat4 projection;

	void setMatrices ();
};

#endif