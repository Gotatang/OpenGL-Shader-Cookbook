#include "tgaio.h"

#include <fstream>
using std::ifstream;
using std::ofstream;

namespace TGAIO
{
	namespace LE
	{
		int readShort ( ifstream &_stream )
		{
			unsigned char ic; // Input char
			int result;

			ic = _stream.get ();
			result = ic;

			ic = _stream.get ();

			result |= (( unsigned int ) ic << 8 );

			return result;
		}

		void writeShort ( ofstream &_stream, int _value )
		{
			unsigned char lowOrder = ( unsigned char ) _value;				// 1st value
			unsigned char highOrder = ( unsigned char ) ( _value >> 8 );	// 2nd value

			_stream.put ( lowOrder );
			_stream.put ( highOrder );
		}

		int readInt ( ifstream &_stream )
		{
			int result = 0;
			unsigned char ic = 0;

			ic = _stream.get ();
			result = ic;

			ic = _stream.get ();
			result |= (( unsigned int ) ic << 8 );

			ic = _stream.get ();
			result |= (( unsigned int ) ic << 16 );

			ic = _stream.get ();
			result |= (( unsigned int ) ic << 24 );

			return result;
		}
	}; // Namespace LE

	GLubyte* read ( const char* _filename, int &_width, int &_height )
	{
		// Open file for reading
		std::ifstream inFile ( _filename, std::ios::binary );

		try
		{
			if ( !inFile )
			{
				std::string msg = std::string ( "Error : can't open " ) + _filename;
				throw IOException ( msg );
			}

			int idLen = inFile.get ();						// Length of image ID field
			int mapType = inFile.get ();					// Color map type ( expect 0 == no color map )
			int typeCode = inFile.get ();					// Image type code ( expect 2 == uncompressed )
			
			inFile.ignore ( 5 );							// Color map info ( ignored )

			int xOrigin = LE::readShort ( inFile );			// X origin
			int yOrigin = LE::readShort ( inFile );			// Y origin

			_width = LE::readShort ( inFile );				// Image width
			_height = LE::readShort ( inFile );				// Image height

			int bpp = inFile.get ();						// Bits per pixel ( expect 24 or 32 )

			inFile.ignore ();								// Image descriptor ( expect 0 for 24 bpp and  8 for 32 bpp )

			if ( typeCode != 2 || mapType != 0 )
			{
				throw IOException ( "File does not appear to be a non-color-mapped, uncompressed TGA image" );
			}

			if ( bpp != 24 && bpp != 32 )
			{
				throw IOException ( "File must be 24 or 32 bits per pixel.\n" );
			}

			// Skip the image ID data
			if ( idLen > 0 )
			{
				inFile.ignore ( idLen );
			}

			// Color map data would be here, but we assume no color map
			printf ( "%s : ( %d x %d ) %d bpp origin ( %d, %d )\n", _filename, _width, _height, bpp, xOrigin, yOrigin );

			// Read pixel data
			GLubyte* p = new GLubyte [ _width * _height * 4 ];
			// 24 bpp -- Blue, Green, Red
			// 32 bpp -- Blue, Green, Red, Alpha
			// p -- stored as RGBA

			for ( unsigned int i = 0 ; i < ( unsigned int ) ( _width * _height ) ; i ++ )
			{
				p [ i * 4 + 2 ] = inFile.get ();		// Blue
				p [ i * 4 + 1 ] = inFile.get ();		// Green
				p [ i * 4 + 0 ] = inFile.get ();		// Red

				if ( bpp == 32 )
				{
					p [ i * 4 + 3 ] = inFile.get ();	// Alpha
				}

				else
				{
					p [ i * 4 + 3 ] = 0xFF;	// Alpha
				}
			}

			inFile.close ();
			return p;
		}

		catch ( IOException &e )
		{
			inFile.close ();
			throw e;
		}
	}

	void write ( GLubyte* _p, int _width, int _height, const char* _filename )
	{
		ofstream oFile ( _filename, std::ios::binary );

		try
		{
			if ( !oFile )
			{
				std::string msg = std::string ( "Unable to open file " ) + _filename + std::string ( " for writing." );
				throw IOException ( msg );
			}

			const char zero [] = { 0, 0, 0, 0, 0 };

			oFile.put ( 0 );						// Lenght of the image ID field
			oFile.put ( 0 );						// Color map type ( 0 == no color map )
			oFile.put ( 2 );						// Imgae type code ( 2 == uncompressed )
			oFile.write ( zero, 5 );				// Color map info ( ignored )
			oFile.write ( zero, 2 );				// X origin ( 0 )
			oFile.write ( zero, 2 );				// Y origin ( 0 )
			LE::writeShort ( oFile, _width );		// Image width
			LE::writeShort ( oFile, _height );		// Image height
			oFile.put ( 32 );						// Bits per pixel ( 32 )
			oFile.put ( 8 );						// Image descriptor ( 8 );

			for ( int i = 0 ; i < ( _width * _height ) ; i ++ )
			{
				oFile.put ( _p [ i * 4 + 2 ]);		// Blue
				oFile.put ( _p [ i * 4 + 1 ]);		// Green
				oFile.put ( _p [ i * 4	   ]);		// Red
				oFile.put ( _p [ i * 4 + 3 ]);		// Alpha
			}

			oFile.close ();
		}

		catch ( IOException &e )
		{
			oFile.close ();
			throw e;
		}
	}

	GLuint loadTexture ( const char* _filename, GLint &_width, GLint &_height )
	{
		GLubyte* data = TGAIO::read ( _filename, _width, _height );

		GLuint textureID;
		glGenTextures ( 1, &textureID );

		glBindTexture ( GL_TEXTURE_2D, textureID );

		// Allocate storage
		glTexStorage2D ( GL_TEXTURE_2D, 1, GL_RGBA8, _width, _height );

		// Copy data into storage
		glTexSubImage2D ( GL_TEXTURE_2D, 0, 0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, data );

		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

		delete [] data;

		return textureID;
	}

	GLuint loadTexture ( const char* _filename )
	{
		GLint w, h;

		return TGAIO::loadTexture ( _filename, w, h );
	}
}; // Namespace TGAIO