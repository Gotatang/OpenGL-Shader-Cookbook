#ifndef __GLUTILS_H_
#define __GLUTILS_H_


#include "GLBasics.h"

namespace GLUtils
{
	void APIENTRY debugCallBack ( GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar *_message, const void *_userParam );

	void dumpGLInfo ( bool _extensions = false );
}

#endif