#ifndef __SCENECARTOON_H_
#define __SCENECARTOON_H_

#include "GLBasics.h"

#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vboTorus.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneCartoon : public Scene
{

public:

	SceneCartoon ();
	~SceneCartoon ();

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