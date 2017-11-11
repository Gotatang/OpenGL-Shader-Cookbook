#include "GLSLProgram.h"

#include <sstream>
#include <fstream>
#include <sys\stat.h>

using std::ifstream;
using std::ios;


namespace GLSLShaderInfo
{
	struct shader_file_extension
	{
		const char* extension;
		GLSLShader::GLSLShaderType type;
	};

	shader_file_extension extensions [] =
	{
		{ ".vs", GLSLShader::GLSLShaderType::VERTEX },
		{ ".vert", GLSLShader::GLSLShaderType::VERTEX },
		{ ".fs", GLSLShader::GLSLShaderType::FRAGMENT },
		{ ".frag", GLSLShader::GLSLShaderType::FRAGMENT },
		{ ".gs", GLSLShader::GLSLShaderType::GEOMETRY },
		{ ".geom", GLSLShader::GLSLShaderType::GEOMETRY },
		{ ".tcs", GLSLShader::GLSLShaderType::TESS_CONTROL },
		{ ".tes", GLSLShader::GLSLShaderType::TESS_EVALUATION },
		{ ".cs", GLSLShader::GLSLShaderType::COMPUTE },
	};
};


GLSLProgram::GLSLProgram () : handle (0), linked (false)
{}

GLSLProgram::~GLSLProgram ()
{
	if ( handle == 0 )
		return;

	// Query the number of attached shaders
	GLint numShaders = 0;
	glGetProgramiv ( handle, GL_ATTACHED_SHADERS, &numShaders );

	// Get the shader names
	GLuint *shaderNames = new GLuint [ numShaders ];
	glGetAttachedShaders ( handle, numShaders, NULL, shaderNames );

	// Delete the shaders
	for ( int i = 0 ; i < numShaders ; i ++ )
	{
		glDeleteShader ( shaderNames [ i ]);
	}

	// Delete the program
	glDeleteProgram ( handle );

	delete [] shaderNames;
}


void GLSLProgram::CompileShader ( const char* _filename ) throw ( GLSLProgramException )
{
	int numExts = sizeof ( GLSLShaderInfo::extensions ) / sizeof ( GLSLShaderInfo::shader_file_extension );
	
	// Check filename extension to determine the shader type
	string ext = getExtension ( _filename );

	GLSLShader::GLSLShaderType type = GLSLShader::VERTEX;

	bool matchFound = false;

	for ( int i = 0 ; i < numExts ; i ++ )
	{
		if ( ext == GLSLShaderInfo::extensions [ i ].extension )
		{
			matchFound = true;
			type = GLSLShaderInfo::extensions [ i ].type;
			break;
		}
	}

	// If we didn't find a match, throw an exception
	if ( matchFound == false )
	{
		string msg = "Unrecognized extension : " + ext + '\n';
		throw GLSLProgramException ( msg );
	}

	// Pass the discovered shader type along
	CompileShader ( _filename, type );
}

void GLSLProgram::CompileShader ( const char* _filename, GLSLShader::GLSLShaderType _type ) throw ( GLSLProgramException )
{
	if ( !fileExists ( _filename ))
	{
		string message = string ( "Unable to open : " ) + _filename + " not found.\n";
	}

	if ( handle <= 0 )
	{
		handle = glCreateProgram ();

		if ( handle == 0 )
		{
			throw GLSLProgramException ( "Unable to create shader program.\n" );
		}
	}

	ifstream inFile ( _filename, ios::in );

	if ( !inFile )
	{
		string message = string ( "Unable to open " ) + _filename; + '\n';
		throw GLSLProgramException ( message );
	}

	// Get file contents
	std::stringstream code;
	code << inFile.rdbuf ();
	inFile.close ();

	CompileShader ( code.str (), _type, _filename );
}

void GLSLProgram::CompileShader ( string &_source, GLSLShader::GLSLShaderType _type, const char* _filename ) throw ( GLSLProgramException )
{
	if ( handle <= 0 )
	{
		handle = glCreateProgram ();

		if ( handle == 0 )
		{
			throw GLSLProgramException ( "Unable to create shader program.\n" );
		}
	}

	GLuint shaderHandle = glCreateShader ( _type );

	const char* code = _source.c_str ();
	glShaderSource ( shaderHandle, 1, &code, NULL );

	// Compile shader
	glCompileShader ( shaderHandle );

	// Check for errors
	int results;

	glGetShaderiv ( shaderHandle, GL_COMPILE_STATUS, &results );

	if ( results == GL_FALSE )
	{
		// Compile failed, get log
		int length = 0;
		string logString;

		glGetShaderiv ( shaderHandle, GL_INFO_LOG_LENGTH, &length );

		if ( length > 0 )
		{
			char* log = new char [ length ];
			int written = 0;

			glGetShaderInfoLog ( shaderHandle, length, &written, log );
			logString = log;

			delete [] log;
		}

		string msg;

		if ( _filename )
		{
			msg = string ( _filename ) + " : shader compilation failed\n";
		}

		else
		{
			msg = "Shader compilation failed.\n";
		}

		msg += logString;

		throw GLSLProgramException ( msg );
	}

	else
	{
		// Compile succeeded, attach shader
		glAttachShader ( handle, shaderHandle );
	}
}


void GLSLProgram::Link () throw ( GLSLProgramException )
{
	if ( linked )
		return;

	if ( handle <= 0 )
	{
		throw GLSLProgramException ( "Program has not been compiled.\n" );
	}

	glLinkProgram ( handle );

	int status = 0;
	glGetProgramiv ( handle, GL_LINK_STATUS, &status );

	if ( status == GL_FALSE )
	{
		// Store log and return false
		int length = 0;
		string logStr;

		glGetProgramiv ( handle, GL_INFO_LOG_LENGTH, &length);

		if ( length > 0 )
		{
			char* cLog = new char [ length ];
			int written = 0;

			glGetProgramInfoLog ( handle, length, &written, cLog );

			logStr = cLog;

			delete [] cLog;
		}

		throw GLSLProgramException ( string ( "Program link failed : \n" ) + logStr );
	}

	else
	{
		uniformLocations.clear ();
		linked = true;
	}
}

void GLSLProgram::Use () throw ( GLSLProgramException )
{
	if ( handle <= 0 || ( !linked ))
	{
		throw GLSLProgramException ( "Shader has not been linked\n" );
	}

	glUseProgram ( handle );
}

void GLSLProgram::Validate () throw ( GLSLProgramException )
{
	if ( !isLinked ())
	{
		throw GLSLProgramException ( "Program is not linked\n" );
	}

	GLint status;
	glValidateProgram ( handle );
	glGetProgramiv ( handle, GL_VALIDATE_STATUS, &status );

	if ( status == GL_FALSE )
	{
		// Store log and return false
		int length = 0;
		string logStr;

		glGetProgramiv ( handle, GL_INFO_LOG_LENGTH, &length );

		if ( length > 0 )
		{
			char* cLog = new char [ length ];
			int written = 0;

			glGetProgramInfoLog ( handle, length, &written, cLog );
			logStr = cLog;

			delete [] cLog;
		}

		throw GLSLProgramException ( string ( "Program failed to validate\n" ) + logStr );
	}
}


int GLSLProgram::getHandle ()
{
	return handle;
}

bool GLSLProgram::isLinked ()
{
	return linked;
}


void GLSLProgram::bindAttribLocation ( GLuint _location, const char* _name )
{
	glBindAttribLocation ( handle, _location, _name );
}

void GLSLProgram::bindFragDataLocation ( GLuint _location, const char* _name )
{
	glBindFragDataLocation ( handle, _location, _name );
}


void GLSLProgram::setUniform ( const char* _name, float _x, float _y, float _z )
{
	GLint loc = getUniformLocation ( _name );
	glUniform3f ( loc, _x, _y, _z );
}

void GLSLProgram::setUniform ( const char* _name, const vec2 &_v )
{
	GLint loc = glGetUniformLocation ( handle, _name );
	glUniform2f ( loc, _v.x, _v.y );
}

void GLSLProgram::setUniform ( const char* _name, const vec3 &_v )
{
	this->setUniform ( _name, _v.x, _v.y, _v.z );
}

void GLSLProgram::setUniform ( const char* _name, const vec4 &_v )
{
	GLint loc = glGetUniformLocation ( handle, _name );
	glUniform4f ( loc, _v.x, _v.y, _v.z, _v.w );
}

void GLSLProgram::setUniform ( const char* _name, const mat3 &_m )
{
	GLint loc = getUniformLocation ( _name );
	glUniformMatrix3fv ( loc, 1, GL_FALSE, &_m [ 0 ][ 0 ]);
}

void GLSLProgram::setUniform ( const char* _name, const mat4 &_m )
{
	GLint loc = getUniformLocation ( _name );
	glUniformMatrix4fv ( loc, 1, GL_FALSE, &_m [ 0 ][ 0 ]);
}

void GLSLProgram::setUniform ( const char* _name, float _value )
{
	GLint loc = getUniformLocation ( _name );
	glUniform1f ( loc, _value );
}

void GLSLProgram::setUniform ( const char* _name, GLuint _value )
{
	GLint loc = getUniformLocation ( _name );
	glUniform1ui ( loc, _value );
}

void GLSLProgram::setUniform ( const char* _name, int _value )
{
	GLint loc = getUniformLocation ( _name );
	glUniform1i ( loc, _value );
}

void GLSLProgram::setUniform ( const char* _name, bool _value )
{
	GLint loc = getUniformLocation ( _name );
	glUniform1i ( loc, _value );
}


void GLSLProgram::printfActiveUniforms ()
{
	GLint numUniforms = 0;
	glGetProgramInterfaceiv ( handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms );

	GLenum properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	printf ( "Active uniforms :\n\n" );

	for ( int i = 0 ; i < numUniforms ; i ++ )
	{
		GLint results [ 4 ];
		glGetProgramResourceiv ( handle, GL_UNIFORM, i, 4, properties, 4, NULL, results );

		// Skip uniforms in blocks
		if ( results [ 3 ] != -1 )
			continue;

		GLint nameBufSize = results [ 0 ] + 1;
		char* name = new char [ nameBufSize ];

		glGetProgramResourceName ( handle, GL_UNIFORM, i, nameBufSize, NULL, name );
		printf ( "%-5d %s (%s)\n", results [ 2 ], name, getTypeString ( results [ 1 ]));
		
		delete [] name;
	}

	printf ( "\n\n" );
}

void GLSLProgram::printfActiveUniformBlocks ()
{
	GLint numBlocks = 0;

	glGetProgramInterfaceiv ( handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks );

	GLenum blockProperties [] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
	GLenum blockIndex [] = { GL_ACTIVE_VARIABLES };
	GLenum properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	// Blocks
	for ( int block = 0 ; block < numBlocks ; block ++ )
	{
		GLint blockInfo [ 2 ];
		glGetProgramResourceiv ( handle, GL_UNIFORM_BLOCK, block, 2, blockProperties, 2, NULL, blockInfo );

		GLint numUniforms = blockInfo [ 0 ];
		char* blockName = new char [ blockInfo [ 1 ] + 1 ];

		glGetProgramResourceName ( handle, GL_UNIFORM_BLOCK, block, blockInfo [ 1 ] + 1, NULL, blockName );
		printf ( "Uniform block \"%s\" :\n\n", blockName );
		delete [] blockName;

		GLint* uniformsIndexes = new GLint [ numUniforms ];
		glGetProgramResourceiv ( handle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUniforms, NULL, uniformsIndexes );

		// Block's uniforms
		for ( int uniform = 0 ; uniform < numUniforms ; uniform ++ )
		{
			GLint uniformIndex = uniformsIndexes [ uniform ];
			GLint results [ 3 ];
			glGetProgramResourceiv ( handle, GL_UNIFORM, uniformIndex, 3, properties, 3, NULL, results );

			GLint nameBufSize = results [ 0 ] + 1;
			char* name = new char [ nameBufSize ];

			glGetProgramResourceName ( handle, GL_UNIFORM, uniformIndex, nameBufSize, NULL, name );
			printf ( "\t%s (%s)\n", name, getTypeString ( results [ 1 ]));

			delete [] name;
		}

		delete [] uniformsIndexes;
	}

	printf ( "\n\n" );
}

void GLSLProgram::printfActiveAttribs ()
{
	GLint numAttribs;
	glGetProgramInterfaceiv ( handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs );

	GLenum properties [] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	printf ( "Active attributes :\n\n" );

	for ( int i = 0 ; i < numAttribs ; i ++ )
	{
		GLint results [ 3 ];
		glGetProgramResourceiv ( handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results );

		GLint nameBufSize = results [ 0 ] + 1;
		char* name = new char [ nameBufSize ];

		glGetProgramResourceName ( handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name );

		printf ( "%-5d %s (%s)\n", results [ 2 ], name, getTypeString ( results [ 1 ]));

		delete [] name;
	}

	printf ( "\n\n" );
}


int GLSLProgram::getUniformLocation ( const char* _name )
{
	std::map <string, int>::iterator pos;
	pos = uniformLocations.find ( _name );

	if ( pos == uniformLocations.end ()) // If the current uniform is not existing
	{
		uniformLocations [ _name ] = glGetUniformLocation ( handle, _name );
	}

	return uniformLocations [ _name ];
}

bool GLSLProgram::fileExists ( const string &_filename )
{
	struct stat info;
	int ret = -1;

	ret = stat ( _filename.c_str (), &info );
	return 0 == ret;
}

string GLSLProgram::getExtension ( const char* _filename )
{
	string filenameStr ( _filename );

	size_t loc = filenameStr.find_last_of ( "." );

	if ( loc != string::npos )
	{
		return filenameStr.substr ( loc, string::npos );
	}

	return "";
}


const char* GLSLProgram::getTypeString ( GLenum _type )
{
	switch ( _type )
	{
		case GL_FLOAT :
			return "float";

		case GL_FLOAT_VEC2 :
			return "vec2";

		case GL_FLOAT_VEC3 :
			return "vec3";

		case GL_FLOAT_VEC4 :
			return "vec4";

		case GL_DOUBLE :
			return "double";

		case GL_INT :
			return "int";

		case GL_UNSIGNED_INT :
			return "unsigned int";

		case GL_BOOL :
			return "bool";

		case GL_FLOAT_MAT2 :
			return "mat2";

		case GL_FLOAT_MAT3 :
			return "mat3";

		case GL_FLOAT_MAT4 :
			return "mat4";

		default:
			return "?";
	}
}