#include "sceneshadowmap.h"


#include <stdio.h>
#include <iostream>

using std::cerr;
using std::endl;


#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"


SceneShadowMap::SceneShadowMap() : m_fTPrev(0.f), m_iWidth(800), m_iHeight(600), m_iShadowMapWidth(8192), m_iShadowMapHeight(8192)
{
}


SceneShadowMap::~SceneShadowMap()
{
	delete m_teapot;
	delete m_plane;
	delete m_torus;
	delete m_lightFrustum;
}


void SceneShadowMap::Init()
{
	CompileAndLinkShader();

	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	glEnable(GL_DEPTH_TEST);

	m_fAngle = glm::quarter_pi<float>();

	m_teapot = new VBOTeapot(14, mat4(1.f));
	m_plane = new VBOPlane(40.f, 40.f, 2, 2);

	float scale = 2.f;
	m_torus = new VBOTorus(0.7f * scale, 0.3f * scale, 50, 50);

	// Set up the framebuffer object
	SetupFBO();

	GLuint programHandle = m_prog.getHandle();
	m_uiPass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
	m_uiPass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shadeWithShadow");

	m_mShadowBias = mat4(vec4(0.5f, 0.f, 0.f, 0.f),
						 vec4(0.f, 0.5f, 0.f, 0.f),
						 vec4(0.f, 0.f, 0.5f, 0.f),
						 vec4(0.5f, 0.5f, 0.5f, 1.f));

	m_lightFrustum = new Frustum(Projection::PERSPECTIVE);
	float c = 1.65f;
	vec3 lightPos = vec3(0.f, c * 5.25f, c * 7.5f); // World coords
	m_lightFrustum->Orient(lightPos, vec3(0.f), vec3(0.f, 1.f, 0.f));
	m_lightFrustum->SetPerspective(50.f, 1.f, 1.f, 25.f);
	m_mLightPV = m_mShadowBias * m_lightFrustum->GetProjectionMatrix() * m_lightFrustum->GetViewMatrix();

	m_prog.setUniform("Light.Intensity", vec3(0.85f));
	m_prog.setUniform("ShadowMap", 0);
}

void SceneShadowMap::SpitOutDepthBuffer()
{
	int size = m_iShadowMapWidth * m_iShadowMapHeight;
	float* buffer = new float[size];
	unsigned char* imgBuffer = new unsigned char[size * 4];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, buffer);

	for (int i = 0; i < m_iShadowMapHeight; ++i)
	{
		for (int j = 0 ; j < m_iShadowMapWidth; ++j)
		{
			int imgIdx = 4 * ((i * m_iShadowMapWidth) + j);
			int bufIdx = ((m_iShadowMapHeight - i - 1) * m_iShadowMapWidth) + j;


			// This is just to make a more visible image.  Scale so that
            // the range (minVal, 1.0) maps to (0.0, 1.0).  This probably should
            // be tweaked for different light configurations.
            float minVal = 0.88f;
            float scale = (buffer[bufIdx] - minVal) / (1.0f - minVal);
            unsigned char val = (unsigned char)(scale * 255);
            imgBuffer[imgIdx] = val;
            imgBuffer[imgIdx+1] = val;
            imgBuffer[imgIdx+2] = val;
            imgBuffer[imgIdx+3] = 0xff;
		}
	}

	delete [] buffer;
	delete [] imgBuffer;
	exit(1);
}

void SceneShadowMap::SetupFBO()
{
	GLfloat border [] = {1.f, 0.f, 0.f, 0.f};

	// The depth buffer texture
	GLuint depthTex;

	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, m_iShadowMapWidth, m_iShadowMapHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	// Assign the depth buffer texture to texture channel 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	// Create and setup the FBO
	glGenFramebuffers(1, &m_uiShadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_uiShadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = {GL_NONE};
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(result == GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer is complete.\n");
	}

	else
	{
		printf("Framebuffer is not complete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneShadowMap::Update(float _t)
{
	float deltaT = _t - m_fTPrev;

	if (m_fTPrev == 0.f)
	{
		deltaT = 0.f;
	}

	m_fTPrev = _t;

	m_fAngle += 0.2f * deltaT;

	if (deltaT > glm::two_pi<float>())
	{
		m_fAngle -= glm::two_pi<float>();
	}
}

void SceneShadowMap::Render()
{
	m_prog.Use();

	// Pass 1 (shadow map generation)
	m_mView = m_lightFrustum->GetViewMatrix();
	m_mProjection = m_lightFrustum->GetProjectionMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, m_uiShadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_iShadowMapWidth, m_iShadowMapHeight);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_uiPass1Index);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	DrawScene();
	glFlush();
	glFinish();
	//SpitOutDepthBuffer(); // This is just used to get an image of the depth buffer

	// Pass 2 (render)
	float c = 1.f;
	vec3 cameraPos(c * 11.5f * cos(m_fAngle), c * 7.f, c * 11.5f * sin(m_fAngle));
	m_mView = glm::lookAt(cameraPos, vec3(0.f), vec3(0.f, 1.f, 0.f));
	m_prog.setUniform("Light.Position", m_mView * vec4(m_lightFrustum->GetOrigin(), 1.f));
	m_mProjection = glm::perspective(glm::radians(50.f), (float)m_iWidth / m_iHeight, 0.1f, 100.f);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, m_iWidth, m_iHeight);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &m_uiPass2Index);
	glDisable(GL_CULL_FACE);
	DrawScene();

	// Uncomment to draw the light's frustum
	//    m_solidProg.Use();
	//    m_solidProg.setUniform("Color", vec4(1.0f,0.0f,0.0f,1.0f));
	//    m_mModel = mat4(1.0f);
	//    mat4 mv = m_mView * m_mModel;
	//    m_solidProg.setUniform("MVP", m_mProjection * mv);
	//    m_lightFrustum->Render();
	//    glFinish();
}

void SceneShadowMap::DrawScene()
{
	vec3 color = vec3(0.7f,0.5f,0.3f);
    m_prog.setUniform("Material.Ka", color * 0.05f);
    m_prog.setUniform("Material.Kd", color);
    m_prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    m_prog.setUniform("Material.Shininess", 150.0f);
    m_mModel = mat4(1.0f);
    m_mModel *= glm::translate(vec3(0.0f,0.0f,0.0f));
    m_mModel *= glm::rotate(glm::radians(-90.0f), vec3(1.0f,0.0f,0.0f));
    SetMatrices();
    m_teapot->Render();

    m_prog.setUniform("Material.Ka", color * 0.05f);
    m_prog.setUniform("Material.Kd", color);
    m_prog.setUniform("Material.Ks", vec3(0.9f,0.9f,0.9f));
    m_prog.setUniform("Material.Shininess", 150.0f);
    m_mModel = mat4(1.0f);
    m_mModel *= glm::translate(vec3(0.0f,2.0f,5.0f));
    m_mModel *= glm::rotate(glm::radians(-45.0f), vec3(1.0f,0.0f,0.0f));
    SetMatrices();
    m_torus->Render();

    m_prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    m_prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    m_prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    m_prog.setUniform("Material.Shininess", 1.0f);
    m_mModel = mat4(1.0f);
    m_mModel *= glm::translate(vec3(0.0f,0.0f,0.0f));
    SetMatrices();
    m_plane->Render();

    m_mModel = mat4(1.0f);
    m_mModel *= glm::translate(vec3(-5.0f,5.0f,0.0f));
    m_mModel *= glm::rotate(glm::radians(-90.0f),vec3(0.0f,0.0f,1.0f));
    SetMatrices();
    m_plane->Render();

    m_mModel = mat4(1.0f);
    m_mModel *= glm::translate(vec3(0.0f,5.0f,-5.0f));
    m_mModel *= glm::rotate(glm::radians(90.0f),vec3(1.0f,0.0f,0.0f));
    SetMatrices();
    m_plane->Render();
    m_mModel = mat4(1.0f);
}

void SceneShadowMap::SetMatrices()
{
	mat4 mv = m_mView * m_mModel;

	m_prog.setUniform("ModelViewMatrix", mv);
	m_prog.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	m_prog.setUniform("MVP", m_mProjection * mv);
	m_prog.setUniform("ShadowMatrix", m_mLightPV * m_mModel);
}

void SceneShadowMap::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_iWidth = _w;
	m_iHeight = _h;
}

void SceneShadowMap::CompileAndLinkShader()
{
	try
	{
		m_prog.CompileShader("shadowmap.vert", GLSLShader::VERTEX);
		m_prog.CompileShader("shadowmap.frag", GLSLShader::FRAGMENT);
		m_prog.Link();
		m_prog.Use();

		m_solidProg.CompileShader("solid.vert", GLSLShader::VERTEX);
		m_solidProg.CompileShader("solid.frag", GLSLShader::FRAGMENT);
		m_solidProg.Link();
	}

	catch(GLSLProgramException& e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}