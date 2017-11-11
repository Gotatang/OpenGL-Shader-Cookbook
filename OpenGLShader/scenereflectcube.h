#ifndef __SCENEREFLECTCUBE_H_
#define __SCENEREFLECTCUBE_H_

#include "GLBasics.h"

#include "scene.h"
#include "GLSLProgram.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "vboTorus.h"
#include "skybox.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneReflectCube : public Scene
{

public:

	SceneReflectCube ();
	~SceneReflectCube ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;
	VBOTeapot* teapot;
	VBOTorus* torus;
	SkyBox* sky;

	mat4 model;
	mat4 view;
	mat4 projection;
	float angle, fprev, rotSpeed;

	void setMatrices ();
	void loadCubeMap ( const char* _baseFileName );
};

#endif