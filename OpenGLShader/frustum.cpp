/*#include "frustum.h"

#include <stdio.h>

#include "glm\gtx\transform.hpp"


Frustum::Frustum(Projection::ProjType _type) : m_type(_type)
{
	GLuint elem[12* 2];

	Orient(vec3 (0.f, 0.f, 1.f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));

	if (m_type == Projection::ORTHOGONAL)
	{
		SetOrthoBounds(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
	}

	else
	{
		SetPerspective(50.f, 1.f, 0.5f, 100.f);

		GLuint idx = 0;

		 // The outer edges
        elem[idx++] = 0; elem[idx++] = 5;
        elem[idx++] = 0; elem[idx++] = 6;
        elem[idx++] = 0; elem[idx++] = 7;
        elem[idx++] = 0; elem[idx++] = 8;
        // The near plane
        elem[idx++] = 1; elem[idx++] = 2;
        elem[idx++] = 2; elem[idx++] = 3;
        elem[idx++] = 3; elem[idx++] = 4;
        elem[idx++] = 4; elem[idx++] = 1;
        // The far plane
        elem[idx++] = 5; elem[idx++] = 6;
        elem[idx++] = 6; elem[idx++] = 7;
        elem[idx++] = 7; elem[idx++] = 8;
        elem[idx++] = 8; elem[idx++] = 5;
	}


	glBindVertexArray(0);
	glGenBuffers(2, m_uiHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiHandle[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLuint), elem, GL_STATIC_DRAW);
}


Frustum::~Frustum(void)
{
}


void Frustum::Orient(const vec3& _vPos, const vec3& _vLookAt, const vec3& _vUp)
{
	m_vOrigin = _vPos;
	m_vLookAt = _vLookAt;
	m_vUp = _vUp;
}

void Frustum::SetOrthoBounds(float _fXMin, float _fXMax, float _fYMin, float _fYMax, float _fNear, float _fFar)
{
	m_fXMin = _fXMin;
	m_fXMax = _fXMax;
	m_fYMin = _fYMin;
	m_fYMax = _fYMax;

	m_fNear = _fNear;
	m_fFar = _fFar;
}

void Frustum::SetPerspective(float _fFOV, float _fAspectRatio, float _fNear, float _fFar)
{
	m_fFOVY = _fFOV;
	m_fAspectRatio = _fAspectRatio;
	m_fNear = _fNear;
	m_fFar = _fFar;
}


void Frustum::Enclose(const Frustum& _other)
{
	vec3 n = glm::normalize(_other.m_vOrigin - _other.m_vLookAt);
    vec3 u = glm::normalize(glm::cross(_other.m_vUp, n));
    vec3 v = glm::normalize(glm::cross(n, u));
    if( m_type == Projection::PERSPECTIVE )
        this->Orient( m_vOrigin, _other.GetCenter(), m_vUp );
    mat4 m = this->GetViewMatrix();

    vec3 p[8];

    // Get 8 points that define the frustum
    if( _other.m_type == Projection::PERSPECTIVE ) {
        float dy = _other.m_fNear * tanf( glm::radians(_other.m_fFOVY) / 2.0f );
		float dx = _other.m_fAspectRatio * dy;
        vec3 c = _other.m_vOrigin - n * _other.m_fNear;
        p[0] = c + u * dx + v * dy;
        p[1] = c - u * dx + v * dy;
        p[2] = c - u * dx - v * dy;
        p[3] = c + u * dx - v * dy;
        dy = _other.m_fFar * tanf( glm::radians(_other.m_fFOVY) / 2.0f );
        dx = _other.m_fAspectRatio * dy;
        c = _other.m_vOrigin - n * _other.m_fFar;
        p[4] = c + u * dx + v * dy;
        p[5] = c - u * dx + v * dy;
        p[6] = c - u * dx - v * dy;
        p[7] = c + u * dx - v * dy;
    } else {
        vec3 c = _other.m_vOrigin - n * _other.m_fNear;
        p[0] = c + u * _other.m_fXMax + v * _other.m_fYMax;
        p[1] = c + u * _other.m_fXMax + v * _other.m_fYMin;
        p[2] = c + u * _other.m_fXMin + v * _other.m_fYMax;
        p[3] = c + u * _other.m_fXMin + v * _other.m_fYMin;
        c = _other.m_vOrigin - n * _other.m_fFar;
        p[4] = c + u * _other.m_fXMax + v * _other.m_fYMax;
        p[5] = c + u * _other.m_fXMax + v * _other.m_fYMin;
        p[6] = c + u * _other.m_fXMin + v * _other.m_fYMax;
        p[7] = c + u * _other.m_fXMin + v * _other.m_fYMin;
    }

    // Adjust frustum to contain
    if( m_type == Projection::PERSPECTIVE ) {
        m_fFOVY = 0.0f;
        m_fFar = 0.0f;
        m_fNear = std::numeric_limits<float>::max();
        float maxHorizAngle = 0.0f;
        for( int i = 0; i < 8; i++) {
            // Convert to local space
            vec4 pt = m * vec4(p[i],1.0f);

            if( pt.z < 0.0f ) {
                float d = -pt.z;
                float angle = atanf( fabs(pt.x) / d );
                if( angle > maxHorizAngle ) maxHorizAngle = angle;
                angle = glm::degrees( atanf( fabs(pt.y) / d ) );
                if( angle * 2.0f > m_fFOVY ) m_fFOVY = angle * 2.0f;
                if( m_fNear > d ) m_fNear = d;
                if( m_fFar < d ) m_fFar = d;
            }
        }
        float h = ( m_fNear * tanf( glm::radians(m_fFOVY)/ 2.0f) ) * 2.0f;
        float w = ( m_fNear * tanf( maxHorizAngle ) ) * 2.0f;
        m_fAspectRatio = w / h;
    } else {
        m_fXMin = m_fYMin = m_fNear = std::numeric_limits<float>::max();
        m_fXMax = m_fYMax = m_fFar = std::numeric_limits<float>::min();
        for( int i = 0; i < 8; i++) {
            // Convert to local space
            vec4 pt = m * vec4(p[i],1.0f);
            if( m_fXMin > pt.x ) m_fXMin = pt.x;
            if( m_fXMax < pt.x ) m_fXMax = pt.x;
            if( m_fYMin > pt.y ) m_fYMin = pt.y;
            if( m_fYMax < pt.y ) m_fYMax = pt.y;
            if( m_fNear > -pt.z ) m_fNear = -pt.z;
            if( m_fFar < -pt.z ) m_fFar = -pt.z;
        }
    }
}


mat4 Frustum::GetViewMatrix() const
{
	return glm::lookAt(m_vOrigin, m_vLookAt, m_vUp);
}

mat4 Frustum::GetProjectionMatrix() const
{
	if (m_type == Projection::PERSPECTIVE)
	{
		return glm::perspective(glm::radians(m_fFOVY), m_fAspectRatio, m_fNear, m_fFar);
	}

	else
	{
		return glm::ortho(m_fXMin, m_fXMax, m_fYMin, m_fYMax, m_fNear, m_fFar);
	}
}

vec3 Frustum::GetOrigin() const
{
	return m_vOrigin;
}

vec3 Frustum::GetCenter() const
{
	float dist = (m_fNear + m_fFar) / 2.f;

	vec3 r = glm::normalize(m_vLookAt - m_vOrigin);

	return m_vOrigin + (r * dist);
}


void Frustum::PrintInfo() const
{
	if (m_type == Projection::PERSPECTIVE)
	{
		printf ("Perspective :	fovy = %f	aspectration	= %f	near = %f	far = %f\n", m_fFOVY, m_fAspectRatio, m_fNear, m_fFar);
	}

	else
	{
		printf ("Orthographic :	x = (%f, %f)	y = (%f, %f)	near = %f	far = %f\n", m_fXMin, m_fXMax, m_fYMin, m_fYMax, m_fNear, m_fFar);
	}

	printf ("	Origin = (%f, %f, %f)	LookAt = (%f, %f, %f)	Up = (%f, %f, %f)\n", m_vOrigin.x, m_vOrigin.y, m_vOrigin.z, m_vLookAt.x, m_vLookAt.y, m_vLookAt.z, m_vUp.x, m_vUp.y, m_vUp.z);
}

void Frustum::Render() const
{
	if (m_type == Projection::PERSPECTIVE)
	{
		static float vert[9 * 3];
		static vec3 p[8];


		vert[0] = m_vOrigin.x;
		vert[1] = m_vOrigin.y;
		vert[2] = m_vOrigin.z;


		vec3 n = glm::normalize(m_vOrigin - m_vLookAt);
		vec3 u = glm::normalize(glm::cross(m_vUp, n));
		vec3 v = glm::normalize(glm::cross(n, u));

		float dy = m_fNear * tanf(glm::radians(m_fFOVY) / 2.f);
		float dx = m_fAspectRatio * dy;
		vec3 c = m_vOrigin - n * m_fNear;

		p[0] = c + u * dx + v * dy;
		p[1] = c - u * dx + v * dy;
		p[2] = c - u * dx - v * dy;
		p[3] = c + u * dx - v * dy;


		dy  = m_fFar * tanf(glm::radians(m_fFOVY) / 2.f);
		dx = m_fAspectRatio * dy;
		c = m_vOrigin - n * m_fFar;

		p[4] = c + u * dx + v * dy;
		p[5] = c - u * dx + v * dy;
		p[6] = c - u * dx - v * dy;
		p[7] = c + u * dx - v * dy;

		int idx = 3;

		for (int i = 0; i < 8; ++i)
		{
			vert[idx++] = p[i].x;
			vert[idx++] = p[i].y;
			vert[idx++] = p[i].z;
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiHandle[0]);
		glBufferData(GL_ARRAY_BUFFER,  9 * 3 * sizeof(float), vert, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);	// Vertex position

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiHandle[1]);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	}
}*/

	#include "frustum.h"
#include "glutils.h"
#include <limits>
#include <cstdio>

#include "glm\gtc\matrix_transform.hpp"

Frustum::Frustum(Projection::ProjType t) : type(t)
{
    GLuint elem[12 * 2];

    this->Orient( vec3(0.0f,0.0f,1.0f), vec3(0.0f,0.0f,0.0f), vec3(0.0f,1.0f,0.0f) );
    if( type == Projection::ORTHO ) {
        this->SetOrthoBounds(-1.0f,1.0f,-1.0f,1.0f,-1.0f,1.0f);
    } else {
        this->SetPerspective(50.0f, 1.0f, 0.5f, 100.0f);

        GLuint idx = 0;
        // The outer edges
        elem[idx++] = 0; elem[idx++] = 5;
        elem[idx++] = 0; elem[idx++] = 6;
        elem[idx++] = 0; elem[idx++] = 7;
        elem[idx++] = 0; elem[idx++] = 8;
        // The near plane
        elem[idx++] = 1; elem[idx++] = 2;
        elem[idx++] = 2; elem[idx++] = 3;
        elem[idx++] = 3; elem[idx++] = 4;
        elem[idx++] = 4; elem[idx++] = 1;
        // The far plane
        elem[idx++] = 5; elem[idx++] = 6;
        elem[idx++] = 6; elem[idx++] = 7;
        elem[idx++] = 7; elem[idx++] = 8;
        elem[idx++] = 8; elem[idx++] = 5;
    }

    glBindVertexArray(0);
    glGenBuffers(2, handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * sizeof(GLuint), elem, GL_STATIC_DRAW);
}

void Frustum::Orient( const vec3 &pos, const vec3& a, const vec3& u )
{
    this->origin = pos;
    this->at = a;
    this->up = u;
}

void Frustum::SetOrthoBounds( float xmin, float xmax, float ymin, float ymax,
                     float nearDist, float farDist )
{
    this->xmin = xmin;
    this->xmax = xmax;
    this->ymin = ymin;
    this->ymax= ymax;
    this->mNear = nearDist;
	this->mFar = farDist;
}

void Frustum::SetPerspective( float fovy, float ar, float nearDist, float farDist )
{
    this->fovy = fovy;
    this->ar = ar;
    this->mNear = nearDist;
    this->mFar = farDist;
}

void Frustum::Enclose( const Frustum & other )
{
    vec3 n = glm::normalize(other.origin - other.at);
    vec3 u = glm::normalize(glm::cross(other.up, n));
    vec3 v = glm::normalize(glm::cross(n, u));
    if( type == Projection::PERSPECTIVE )
        this->Orient( origin, other.GetCenter(), up );
    mat4 m = this->GetViewMatrix();

    vec3 p[8];

    // Get 8 points that define the frustum
    if( other.type == Projection::PERSPECTIVE ) {
        float dy = other.mNear * tanf( glm::radians(other.fovy) / 2.0f );
        float dx = other.ar * dy;
        vec3 c = other.origin - n * other.mNear;
        p[0] = c + u * dx + v * dy;
        p[1] = c - u * dx + v * dy;
        p[2] = c - u * dx - v * dy;
        p[3] = c + u * dx - v * dy;
        dy = other.mFar * tanf( glm::radians(other.fovy) / 2.0f );
        dx = other.ar * dy;
        c = other.origin - n * other.mFar;
        p[4] = c + u * dx + v * dy;
        p[5] = c - u * dx + v * dy;
        p[6] = c - u * dx - v * dy;
        p[7] = c + u * dx - v * dy;
    } else {
        vec3 c = other.origin - n * other.mNear;
        p[0] = c + u * other.xmax + v * other.ymax;
        p[1] = c + u * other.xmax + v * other.ymin;
        p[2] = c + u * other.xmin + v * other.ymax;
        p[3] = c + u * other.xmin + v * other.ymin;
        c = other.origin - n * other.mFar;
        p[4] = c + u * other.xmax + v * other.ymax;
        p[5] = c + u * other.xmax + v * other.ymin;
        p[6] = c + u * other.xmin + v * other.ymax;
        p[7] = c + u * other.xmin + v * other.ymin;
    }

    // Adjust frustum to contain
    if( type == Projection::PERSPECTIVE ) {
        fovy = 0.0f;
        mFar = 0.0f;
        mNear = std::numeric_limits<float>::max();
        float maxHorizAngle = 0.0f;
        for( int i = 0; i < 8; i++) {
            // Convert to local space
            vec4 pt = m * vec4(p[i],1.0f);

            if( pt.z < 0.0f ) {
                float d = -pt.z;
                float angle = atanf( fabs(pt.x) / d );
                if( angle > maxHorizAngle ) maxHorizAngle = angle;
                angle = glm::degrees( atanf( fabs(pt.y) / d ) );
                if( angle * 2.0f > fovy ) fovy = angle * 2.0f;
                if( mNear > d ) mNear = d;
                if( mFar < d ) mFar = d;
            }
        }
        float h = ( mNear * tanf( glm::radians(fovy)/ 2.0f) ) * 2.0f;
        float w = ( mNear * tanf( maxHorizAngle ) ) * 2.0f;
        ar = w / h;
    } else {
        xmin = ymin = mNear = std::numeric_limits<float>::max();
        xmax = ymax = mFar = std::numeric_limits<float>::min();
        for( int i = 0; i < 8; i++) {
            // Convert to local space
            vec4 pt = m * vec4(p[i],1.0f);
            if( xmin > pt.x ) xmin = pt.x;
            if( xmax < pt.x ) xmax = pt.x;
            if( ymin > pt.y ) ymin = pt.y;
            if( ymax < pt.y ) ymax = pt.y;
            if( mNear > -pt.z ) mNear = -pt.z;
            if( mFar < -pt.z ) mFar = -pt.z;
        }
    }

}

mat4 Frustum::GetViewMatrix() const
{
    return glm::lookAt( origin, at, up );
}

mat4 Frustum::GetProjectionMatrix() const
{
    if( type == Projection::PERSPECTIVE )
        return glm::perspective( glm::radians(fovy), ar, mNear, mFar );
    else
        return glm::ortho(xmin, xmax, ymin, ymax, mNear, mFar);
}

vec3 Frustum::GetOrigin() const
{
    return this->origin;
}

vec3 Frustum::GetCenter() const
{
    float dist = (mNear + mFar) / 2.0f;
    vec3 r = glm::normalize(at - origin);

    return origin + (r * dist);
}

void Frustum::printInfo() const
{
    if( type == Projection::PERSPECTIVE ) {
        printf("Perspective:  fovy = %f  ar = %f  near = %f  far = %f\n",
               fovy, ar, mNear, mFar);
    } else {
        printf("Orthographic: x(%f,%f) y(%f,%f) near = %f far = %f\n",
               xmin, xmax, ymin, ymax, mNear, mFar);
    }
    printf("   Origin = (%f, %f, %f)  at = (%f, %f, %f) up = (%f, %f, %f)\n",
           origin.x, origin.y, origin.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

void Frustum::Render() const
{
    if( type == Projection::PERSPECTIVE ) {
        static float vert[9 * 3];
        static vec3 p[8];

        vert[0] = origin.x;
        vert[1] = origin.y;
        vert[2] = origin.z;

        vec3 n = glm::normalize(this->origin - this->at);
        vec3 u = glm::normalize(glm::cross(this->up, n));
        vec3 v = glm::normalize(glm::cross(n, u));

        float dy = mNear * tanf( glm::radians(fovy) / 2.0f );
        float dx = ar * dy;
        vec3 c = origin - n * mNear;  // Center of near plane
        p[0] = c + u * dx + v * dy;
        p[1] = c - u * dx + v * dy;
        p[2] = c - u * dx - v * dy;
        p[3] = c + u * dx - v * dy;
        dy = mFar * tanf( glm::radians(fovy) / 2.0f );
        dx = ar * dy;
        c = origin - n * mFar;      // Center of far plane
        p[4] = c + u * dx + v * dy;
        p[5] = c - u * dx + v * dy;
        p[6] = c - u * dx - v * dy;
        p[7] = c + u * dx - v * dy;

        int idx = 3;
        for( int i = 0; i < 8 ; i++ ) {
            vert[idx++] = p[i].x;
            vert[idx++] = p[i].y;
            vert[idx++] = p[i].z;
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
        glBufferData(GL_ARRAY_BUFFER, 9 * 3 * sizeof(float), vert, GL_DYNAMIC_DRAW);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
        glEnableVertexAttribArray(0);  // Vertex position

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    }
}