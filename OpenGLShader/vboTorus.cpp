#include "vboTorus.h"


#include "glm\gtc\constants.hpp"


VBOTorus::VBOTorus( float _outerRadius, float _innerRadius, int _nsides, int _nrings ) : rings ( _nrings ), sides ( _nsides )
{
	faces = sides * rings;
	int iVertices = sides * ( rings + 1 ); // One extra ring to duplicate first ring

	// Vertices
	float* pVertices = new float [ 3 * iVertices ];

	// Normals
	float* pNormals = new float [ 3 * iVertices ];

	// Tex Coords
	float* pTexCoord = new float [ 2 * iVertices ];

	// Elements
	unsigned int* pElements = new unsigned int [ 6 * faces ];


	// Generate the vertex data
	generateVerts ( pVertices, pNormals, pTexCoord, pElements, _innerRadius, _outerRadius );

	// Create and populate the buffer objects
	unsigned int handle [ 4 ];
	glGenBuffers ( 4, handle );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 0 ]);
	glBufferData ( GL_ARRAY_BUFFER, ( 3 * iVertices ) * sizeof ( float ), pVertices, GL_STATIC_DRAW );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 1 ]);
	glBufferData ( GL_ARRAY_BUFFER, ( 3 * iVertices ) * sizeof ( float ), pNormals, GL_STATIC_DRAW );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 2 ]);
	glBufferData ( GL_ARRAY_BUFFER, ( 2 * iVertices ) * sizeof ( float ), pTexCoord, GL_STATIC_DRAW );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, handle [ 3 ]);
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, ( 6 * faces ) * sizeof ( unsigned int ), pElements, GL_STATIC_DRAW );

	delete [] pVertices;
	delete [] pNormals;
	delete [] pTexCoord;
	delete [] pElements;


	// Create the VAO
	glGenVertexArrays ( 1, &vaoHandle );
	glBindVertexArray ( vaoHandle );

	glEnableVertexAttribArray ( 0 );				// Vertex position
	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 0 ]);
	glVertexAttribPointer ( GLuint (0), 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));

	glEnableVertexAttribArray ( 1 );				// Vertex normal
	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 1 ]);
	glVertexAttribPointer (( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));

	glEnableVertexAttribArray ( 2 );				// Texture coords
	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 2 ]);
	glVertexAttribPointer (( GLuint ) 2, 2, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, handle [ 3 ]);

	glBindVertexArray ( 0 );
}

void VBOTorus::Render () const
{
	glBindVertexArray ( vaoHandle );
	glDrawElements ( GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, (( GLubyte *) NULL + ( 0 )));
}


void VBOTorus::generateVerts ( float* _vertices, float* _normals, float* _texCoord, unsigned int* _elements, float _innerRadius, float _outerRadius )
{
	float ringFactor = glm::two_pi <float> () / rings;
	float sideFactor = glm::two_pi <float> () / sides;
	int idx = 0, tidx = 0;

	for ( int ring = 0 ; ring <= rings ; ring ++ )
	{
		float u = ring * ringFactor;
		float cosU = cos ( u );
		float sinU = sin ( u );

		for ( int side = 0 ; side < sides ; side ++ )
		{
			float v = side * sideFactor;
			float cosV = cos ( v );
			float sinV = sin ( v );
			float r = ( _outerRadius + _innerRadius * cosV );		// Position de la ring 

			// Vertex Coord
			_vertices [ idx ] = r * cosU;					// X
			_vertices [ idx + 1 ] = r * sinU;				// Y
			_vertices [ idx + 2 ] = _innerRadius * sinV;	// Z

			// Per vertex normal
			_normals [ idx ] = cosV * cosU * r;
			_normals [ idx + 1 ] = cosV * sinU * r;
			_normals [ idx + 2 ] = sinV * r;

			// Vertex tex coord
			_texCoord [ tidx ] = u / glm::two_pi <float> ();
			_texCoord [ tidx + 1 ] = v / glm::two_pi <float> ();

			tidx += 2;

			// Normalize
			float len = sqrt (	_normals [ idx ] * _normals [ idx ] +
								_normals [ idx + 1 ] * _normals [ idx + 1 ] +
								_normals [ idx + 2 ] * _normals [ idx + 2 ]);

			_normals [ idx ] /= len;
			_normals [ idx + 1 ] /= len;
			_normals [ idx + 2 ] /= len;

			idx += 3;
		}
	}

	idx = 0;

	for ( int ring = 0 ; ring < rings ; ring ++ )
	{
		int ringStart = ring * sides;
		int nextRingStart = ( ring + 1 ) * sides;

		for ( int side = 0 ; side < sides ; side ++ )
		{
			int nextSide = ( side + 1 )% sides;

			// The quad
			_elements [ idx ] = ( ringStart + side );
			_elements [ idx + 1 ] = ( nextRingStart + side );
			_elements [ idx + 2 ] = ( nextRingStart + nextSide );
			_elements [ idx + 3 ] = ringStart + side;
			_elements [ idx + 4 ] = nextRingStart + nextSide;
			_elements [ idx + 5 ] = ( ringStart + nextSide );

			idx += 6;
		}
	}
}

int VBOTorus::getVertexArrayHandle ()
{
	return vaoHandle;
}