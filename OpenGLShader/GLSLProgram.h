#ifndef __GLSLPROGRAM_H_
#define __GLSLPROGRAM_H_

#ifdef WIN32
#pragma warning( disable : 4290 )
#endif


#include "GLBasics.h"

#include <string>
using std::string;

#include <map>
#include <stdexcept>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


class GLSLProgramException : public std::runtime_error
{

public:

	GLSLProgramException ( const string &msg ) : std::runtime_error ( msg )
	{}
};


namespace GLSLShader
{
	enum GLSLShaderType
	{
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
		COMPUTE = GL_COMPUTE_SHADER,
	};
};


class GLSLProgram
{

public:

	GLSLProgram();
	~GLSLProgram();

	void CompileShader ( const char* _filename ) throw ( GLSLProgramException );
	void CompileShader ( const char* _filename, GLSLShader::GLSLShaderType _type ) throw ( GLSLProgramException );
	void CompileShader ( string &_source, GLSLShader::GLSLShaderType _type, const char* _filename = NULL ) throw ( GLSLProgramException );

	void Link () throw ( GLSLProgramException );
	void Use () throw ( GLSLProgramException );
	void Validate () throw ( GLSLProgramException );

	int getHandle ();
	bool isLinked ();

	void bindAttribLocation ( GLuint _location, const char* _name );
	void bindFragDataLocation ( GLuint _location, const char* _name );

	void setUniform ( const char* _name, float _x, float _y, float _z );
	void setUniform ( const char* _name, const vec2 &_v );
	void setUniform ( const char* _name, const vec3 &_v );
	void setUniform ( const char* _name, const vec4 &_v );
	void setUniform ( const char* _name, const mat3 &_m );
	void setUniform ( const char* _name, const mat4 &_m );
	void setUniform ( const char* _name, float _value );
	void setUniform ( const char* _name, bool _value );
	void setUniform ( const char* _name, int _value );
	void setUniform ( const char* _name, GLuint _value );

	void printfActiveUniforms ();
	void printfActiveAttribs ();
	void printfActiveUniformBlocks ();

private:

	GLuint handle;
	bool linked;

	std::map < string, int > uniformLocations;

	int getUniformLocation ( const char* _uniform );
	bool fileExists ( const string &_filename );
	string getExtension ( const char* _filename );

	const char* getTypeString ( GLenum _type );
};

#endif