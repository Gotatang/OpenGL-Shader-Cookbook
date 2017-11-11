#ifndef __TGAIO_H_
#define __TGAIO_H_

#include "GLBasics.h"

#include <stdexcept>
#include <string>

class ifstream;
class ofstream;

namespace TGAIO
{
	class IOException : public std::runtime_error
	{
		
	public :
		IOException ( const std::string &_msg ) : std::runtime_error ( _msg )
		{};
	};

	namespace LE
	{
		/*
		Read a little-endian short ( 2 bytes ) from stream and return
		the value as an int.
		@param stream the ifstream to read from.
		*/
		int readShort ( ifstream &_stream );

		void writeShort ( ofstream &_stream, int _value );

		/*
		Read a little-endian int ( 4 bytes ) from stream and return
		the value.
		@param stream the ifstream to read from.
		*/
		int readInt ( ifstream &_stream );
	};

	GLubyte* read ( const char* _filename, /*out*/ int &_width, /*out*/ int &_height );
	void write ( GLubyte* _pixelData, int _width, const char* _filename );

	GLuint loadTexture ( const char* _filename, GLint &_width /*out*/, GLint &_height /*out*/ );

	/*
	Loads a TGA file into an OpenGL texture. This methods only supports 24 or 32 bpp images.
	@param filename the file name of the TGA file.
	@return the texture ID
	*/
	GLuint loadTexture ( const char* _filename );
};

#endif