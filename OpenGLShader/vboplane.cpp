#include "vboplane.h"

#include "GLBasics.h"


VBOPlane::VBOPlane ( float _xSize, float _zSize, int _xDivs, int _zDivs, float _sMax, float _tMax )
{
	faces = _xDivs * _zDivs;

	float* vertices = new float [ ( _xDivs + 1 ) * ( _zDivs + 1 ) * 3 ];
	float* normals = new float [ ( _xDivs + 1 ) * ( _zDivs + 1 ) * 3 ];
	float* texCoords = new float [ ( _xDivs + 1 ) * ( _zDivs + 1 ) * 2 ];
	unsigned int* elements = new unsigned int [ _xDivs * _zDivs * 6 ];

	float x2 = _xSize / 2.f;
	float z2 = _zSize / 2.f;
	float iFactor = ( float ) _zSize / _zDivs;
	float jFactor = ( float ) _xSize / _xDivs;
	float texi = _sMax / _zDivs;
	float texj = _tMax / _xDivs;
	float x, z;

	int vidx = 0, tidx = 0;

	for ( int i = 0 ; i <= _zDivs ; i ++ )
	{
		z = iFactor * i - z2;

		for ( int j = 0 ; j <= _xDivs ; j ++ )
		{
			x = jFactor * j - x2;

			vertices [ vidx ] = x;
			vertices [ vidx + 1 ] = 0.f;
			vertices [ vidx + 2 ] = z;

			normals [ vidx ] = 0.f;
			normals [ vidx + 1 ] = 1.f;
			normals [ vidx + 2 ] = 0.f;

			texCoords [ tidx ] = j * texi;
			texCoords [ tidx + 1 ] = i * texj;

			vidx += 3;
			tidx += 2;
		}
	}

	unsigned int rowStart, nextRowStart;
	int idx = 0;
	
	// Quads
	for ( int i = 0 ; i < _zDivs ; i ++ ) // Rows
	{
		rowStart = i * ( _xDivs + 1 );
		nextRowStart = ( i + 1 ) * ( _xDivs + 1 );

		for ( int j = 0 ; j < _xDivs ; j ++ )
		{
			elements [ idx ] = rowStart + j;
			elements [ idx + 1 ] = nextRowStart + j;
			elements [ idx + 2 ] = nextRowStart + j + 1;

			elements [ idx + 3 ] = rowStart + j;
			elements [ idx + 4 ] = nextRowStart + j + 1;
			elements [ idx + 5 ] = rowStart + j + 1;

			idx += 6;
		}
	}

	unsigned int handle [ 4 ];
	glGenBuffers ( 4, handle );

	glGenVertexArrays ( 1, &vaoHandle );
	glBindVertexArray ( vaoHandle );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 0 ]);					// Vertex position
	glBufferData ( GL_ARRAY_BUFFER, 3 * ( _xDivs + 1 ) * ( _zDivs + 1 ) * sizeof ( float ), vertices, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 0, 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 0 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 1 ]);					// Vertex normal
	glBufferData ( GL_ARRAY_BUFFER, 3 * ( _xDivs + 1 ) * ( _zDivs + 1 ) * sizeof ( float ), normals, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 1 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 2 ]);					// Vertex texture coordinates
	glBufferData ( GL_ARRAY_BUFFER, 2 * ( _xDivs + 1 ) * ( _zDivs + 1 ) * sizeof ( float ), texCoords, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 2, 2, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 2 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, handle [ 3 ]);
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 6 * _xDivs * _zDivs * sizeof ( unsigned int ), elements, GL_STATIC_DRAW );

	glBindVertexArray ( 0 );

	delete [] vertices;
	delete [] normals;
	delete [] texCoords;
	delete [] elements;
}


void VBOPlane::Render () const
{
	glBindVertexArray ( vaoHandle );
	glDrawElements ( GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, (( GLubyte *) NULL + ( 0 )));
}