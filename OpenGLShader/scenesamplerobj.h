#ifndef __SCENESAMPLEROBJ_H_
#define __SCENESAMPLEROBJ_H_


#include "Scene.h"

#include "GLSLProgram.h"
#include "vboplane.h"

#include "glm\glm.hpp"
using glm::mat4;

class SceneSamplerObj : public Scene
{

public:

	SceneSamplerObj ();

	~SceneSamplerObj ();

	void  Init ();
	void Update ( float t );
	void Render ();

private:

	GLSLProgram shader;

	VBOPlane* plane;

	mat4 model;
	mat4 view;
	mat4 projection;

	float angle;

	GLuint linearSampler, nearestSampler;


	void setMatrices ();
};

#endif