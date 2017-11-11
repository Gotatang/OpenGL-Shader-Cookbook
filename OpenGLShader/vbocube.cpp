#include "vbocube.h"


VBOCube::VBOCube ()
{
	float side = 1.f;
	float side2 = side / 2.f;

	float vertices [ 24 * 3 ] =
	{
		// Front
		-side2, -side2,  side2,
		 side2, -side2,  side2,
		 side2,  side2,  side2,
		-side2,  side2,  side2,

		// Right
		 side2, -side2,  side2,
		 side2, -side2, -side2,
		 side2,  side2, -side2,
		 side2,  side2,  side2,

		 // Back
		-side2, -side2, -side2,
		-side2,  side2, -side2,
		 side2,  side2, -side2,
		 side2, -side2, -side2,

		 // Left
		-side2, -side2,  side2,
		-side2,  side2,  side2,
		-side2,  side2, -side2,
		-side2, -side2, -side2,

		// Bottom
		-side2, -side2,  side2,
		-side2, -side2, -side2,
		 side2, -side2, -side2,
		 side2, -side2,  side2,

		 // Top
		-side2,  side2,  side2,
		 side2,  side2,  side2,
		 side2,  side2, -side2,
		-side2,  side2, -side2,
	};

	float normals [ 24 * 3 ] =
	{
		// Front
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,

		// Right
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,

		 // Back
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,

		 // Left
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,

		// Bottom
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,

		// Top
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};

	float textureCoords [ 24 * 2 ] =
	{
		// Front
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,

		// Right
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,

		 // Back
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,

		 // Left
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,

		// Bottom
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f,

		// Top
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	GLuint elements [] =
	{
		0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9, 10, 8, 10, 11,
		12, 13, 14, 12, 14, 15,
		16, 17, 18, 16, 18, 19,
		20, 21, 22, 20, 22, 23
	};

	glGenVertexArrays ( 1, &vaoHandle );
	glBindVertexArray ( vaoHandle );

	unsigned int handle [ 4 ];
	glGenBuffers ( 4, handle );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 0 ]);				// Vertex positions
	glBufferData ( GL_ARRAY_BUFFER, 24 * 3 * sizeof ( float ), vertices, GL_STATIC_DRAW );
	glVertexAttribPointer ( GLuint ( 0 ), 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 0 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 1 ]);				// Vertex normals
	glBufferData ( GL_ARRAY_BUFFER, 24 * 3 * sizeof ( float ), normals, GL_STATIC_DRAW );
	glVertexAttribPointer ( GLuint ( 1 ), 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 1 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 2 ]);				// Vertex texture coordinates
	glBufferData ( GL_ARRAY_BUFFER, 24 * 2 * sizeof ( float ), textureCoords, GL_STATIC_DRAW );
	glVertexAttribPointer ( GLuint ( 2 ), 2, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 2 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, handle [ 3 ]);
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof ( GLuint ), elements, GL_STATIC_DRAW );

	glBindVertexArray ( 0 );
}

void VBOCube::Render () const
{
	glBindVertexArray ( vaoHandle );
	glDrawElements ( GL_TRIANGLES, 36, GL_UNSIGNED_INT, (( GLubyte* ) NULL + ( 0 )));
}