#ifndef __SCENESHADOWMAP_H_
#define __SCENESHADOWMAP_H_

#include "scene.h"
#include "GLSLProgram.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vboTorus.h"
#include "vboteapot.h"
#include "frustum.h"


#include "glm\glm.hpp"
using glm::vec3;
using glm::vec4;
using glm::mat4;


class SceneShadowMap : public Scene
{

public:

	SceneShadowMap();
	~SceneShadowMap();


	void Init();
	void Update(float _t);
	void Render();

	void Resize(int _w, int _h);


private:

	void SetMatrices();
	void CompileAndLinkShader();
	void SetupFBO();
	void DrawScene();
	void SpitOutDepthBuffer();


	GLSLProgram				m_prog, m_solidProg;
	GLuint					m_uiShadowFBO, m_uiPass1Index, m_uiPass2Index;

	VBOTeapot*				m_teapot;
	VBOPlane*				m_plane;
	VBOTorus*				m_torus;

	int						m_iWidth, m_iHeight;
	int						m_iShadowMapWidth, m_iShadowMapHeight;
	float					m_fTPrev;

	mat4					m_mModel, m_mView, m_mProjection;
	mat4					m_mLightPV;
	mat4					m_mShadowBias;
	float					m_fAngle;

	Frustum*				m_lightFrustum;

};

#endif !__SCENESHADOWMAP_H_