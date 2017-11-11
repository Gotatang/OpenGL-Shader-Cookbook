#include "GLUtils.h"


#include <stdio.h>
#include <string>

using std::string;

namespace GLUtils
{
	void APIENTRY debugCallBack ( GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const GLchar *_message, const void *_userParam )
	{
		// Convert GLenum parameters to strings
		string sourceStr;
		switch ( _source )
		{
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM :
			{
				sourceStr = "WindowSys";
				break;
			}

			case GL_DEBUG_SOURCE_APPLICATION :
			{
				sourceStr = "App";
				break;
			}

			case GL_DEBUG_SOURCE_API :
			{
				sourceStr = "OpenGL";
				break;
			}

			case GL_DEBUG_SOURCE_SHADER_COMPILER :
			{
				sourceStr = "ShaderCompiler";
				break;
			}

			case GL_DEBUG_SOURCE_THIRD_PARTY :
			{
				sourceStr = "3rdParty";
				break;
			}

			case GL_DEBUG_SOURCE_OTHER :
			{
				sourceStr = "Other";
				break;
			}

			default :
			{
				sourceStr = "Unknown";
			}
		}


		string typeStr;
		switch ( _type )
		{
			case GL_DEBUG_TYPE_ERROR :
			{
				typeStr = "Error";
				break;
			}

			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR :
			{
				typeStr = "Deprecated";
				break;
			}

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR :
			{
				typeStr = "Undefined";
				break;
			}

			case GL_DEBUG_TYPE_PORTABILITY :
			{
				typeStr = "Portability";
				break;
			}

			case GL_DEBUG_TYPE_PERFORMANCE :
			{
				typeStr = "Performance";
				break;
			}

			case GL_DEBUG_TYPE_MARKER :
			{
				typeStr = "Marker";
				break;
			}

			case GL_DEBUG_TYPE_PUSH_GROUP :
			{
				typeStr = "PushGrp";
				break;
			}

			case GL_DEBUG_TYPE_POP_GROUP :
			{
				typeStr = "PopGrp";
				break;
			}

			case GL_DEBUG_TYPE_OTHER :
			{
				typeStr = "Other";
				break;
			}

			default:
			{
				typeStr = "Unknown";
			}
		}

		
		string severityStr;
		switch ( _severity )
		{
			case GL_DEBUG_SEVERITY_HIGH :
			{
				severityStr = "HIGH";
				break;
			}

			case GL_DEBUG_SEVERITY_MEDIUM :
			{
				severityStr = "MED";
				break;
			}

			case GL_DEBUG_SEVERITY_LOW :
			{
				severityStr = "LOW";
				break;
			}

			case GL_DEBUG_SEVERITY_NOTIFICATION :
			{
				severityStr = "NOTIFY";
				break;
			}

			default:
			{
				severityStr = "UNK";
			}
		}

		printf ( "%s : %s [%s] (%d) : %s\n", sourceStr.c_str (), typeStr.c_str (), severityStr.c_str (), _id, _message );
	}

	void dumpGLInfo ( bool _extensions )
	{
		const GLubyte *renderer = glGetString ( GL_RENDERER );
		const GLubyte *vendor = glGetString ( GL_VENDOR );
		const GLubyte *version = glGetString ( GL_VERSION );
		const GLubyte *glslVersion = glGetString ( GL_SHADING_LANGUAGE_VERSION );

		GLint major, minor;

		glGetIntegerv ( GL_MAJOR_VERSION, &major );
		glGetIntegerv ( GL_MINOR_VERSION, &minor );

		printf ( "GL Render\t\t\t%s\n", renderer );
		printf ( "GL Vendor\t\t\t%s\n", vendor );
		printf ( "GL Version\t\t\t%s\n", version );
		printf ( "GL GLSL Version\t\t\t%s\n\n\n", glslVersion );

		if ( _extensions )
		{
			GLint nExtensions;
			glGetIntegerv ( GL_NUM_EXTENSIONS, &nExtensions );

			for ( int i = 0 ; i < nExtensions ; i ++ )
			{
				printf ( "%s\n", glGetStringi ( GL_EXTENSIONS, i ));
			}
		}
	}
}