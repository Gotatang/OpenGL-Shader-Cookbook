#ifndef __SCENESPOTLIGHT_H_
#define __SCENESPOTLIGHT_H_

#include "GLBasics.h"

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vboTorus.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneSpotlight : public Scene
{

public:

	SceneSpotlight ();
	~SceneSpotlight ();

	void Init  ();
	void Update ( float t );
	void Render ();


private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOTeapot* teapot;
	VBOTorus* torus;

	mat4 model;
	mat4 view;
	mat4 projection;
	float fPrev;
	float angle;

	void setMatrices ();
};

#endif