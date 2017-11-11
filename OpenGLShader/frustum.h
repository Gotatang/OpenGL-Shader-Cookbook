/*#ifndef __FRUSTUM_H_
#define __FRUSTUM_H_

#include "GLBasics.h"
#include "glm\glm.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Projection
{
	enum ProjType
	{
		PERSPECTIVE,
		ORTHOGONAL,
	};
}


class Frustum
{

public:

	Frustum(Projection::ProjType _type);
	~Frustum();


	void Orient(const vec3& _vPos, const vec3& _vLookAt, const vec3& _vUp);
	void SetOrthoBounds(float _fXMin, float _fXMax, float _fYMin, float _fYMax, float _fNear, float _fFar);
	void SetPerspective(float _fFOV, float _fAspectRatio, float _fNear, float _fFar);
	void Enclose(const Frustum& _other);

	mat4 GetViewMatrix() const;
	mat4 GetProjectionMatrix() const;
	vec3 GetOrigin() const;
	vec3 GetCenter() const;

	void PrintInfo() const;
	void Render() const;


private:

	Projection::ProjType		m_type;

	vec3						m_vOrigin;
	vec3						m_vLookAt;
	vec3						m_vUp;

	float						m_fNear;
	float						m_fFar;
	float						m_fXMin, m_fXMax, m_fYMin, m_fYMax;
	float						m_fFOVY, m_fAspectRatio;

	mat4						m_mView, m_mPro;

	GLuint						m_uiHandle [2];

};

#endif !__FRUSTUM_H_*/

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "GLBasics.h"

#include "glm\glm.hpp"
using glm::vec3;
using glm::vec4;
using glm::mat4;

namespace Projection {
    enum ProjType {
        PERSPECTIVE, ORTHO
    };
}

class Frustum
{
private:
    Projection::ProjType type;

    vec3 origin;
    vec3 at;
    vec3 up;

    float mNear;
    float mFar;
    float xmin, xmax, ymin, ymax;
    float fovy, ar;

    mat4 view, proj;
    GLuint handle[2];

public:
    Frustum( Projection::ProjType type );

    void Orient( const vec3 &pos, const vec3& a, const vec3& u );
    void SetOrthoBounds( float xmin, float xmax, float ymin, float ymax,
                         float , float  );
    void SetPerspective( float , float , float , float  );
    void Enclose( const Frustum & );

    mat4 GetViewMatrix() const;
    mat4 GetProjectionMatrix() const;
    vec3 GetOrigin() const;
    vec3 GetCenter() const;

    void printInfo() const;
    void Render() const;
};

#endif // FRUSTUM_H