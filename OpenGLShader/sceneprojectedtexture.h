#ifndef __SCENEPROJECTEDTEXTURE_H_
#define __SCENEPROJECTEDTEXTURE_H_

#include "GLBasics.h"
#include "scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"
#include "vboteapot.h"
#include "skybox.h"

#include "glm\glm.hpp"

class SceneProjectedTexture : public Scene
{

public:

	SceneProjectedTexture ();
	~SceneProjectedTexture ();

	void Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane *plane;
	VBOTeapot *teapot;
	SkyBox *sky;

	mat4 model;
	mat4 view;
	mat4 projection;

	float fPrev, angle, rotSpeed;

	void setMatrices ();

};

#endif