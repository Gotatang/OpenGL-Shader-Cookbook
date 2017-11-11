#include "vboteapot.h"

#include "teapotdata.h"

#include "glm\gtc\matrix_transform.hpp"

using glm::mat4;
using glm::vec4;


VBOTeapot::VBOTeapot ( int _grid, const glm::mat4& _lidTranform )
{
	int iVertices = 32 * ( _grid + 1 ) * ( _grid + 1 );
	faces = _grid * _grid * 32;
	float* vertices = new float [ iVertices * 3 ];
	float* normals = new float [ iVertices * 3 ];
	float* texCoords = new float [ iVertices * 2 ];
	unsigned int* elements = new unsigned int [ faces * 6 ];

	glGenVertexArrays ( 1, &vaoHandle );
	glBindVertexArray ( vaoHandle );

	unsigned int handle [ 4 ];
	glGenBuffers ( 4, handle );

	GeneratePatches ( vertices, normals, texCoords, elements, _grid );
	MoveLid ( _grid, vertices, _lidTranform );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 0 ]);						// Vertex position
	glBufferData ( GL_ARRAY_BUFFER, ( 3 * iVertices ) * sizeof ( float ), vertices, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 0, 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 0 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 1 ]);						// Vertex normals
	glBufferData ( GL_ARRAY_BUFFER, ( 3 * iVertices ) * sizeof ( float ), normals, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 1, 3, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 1 );

	glBindBuffer ( GL_ARRAY_BUFFER, handle [ 2 ]);						// Texture coordinates
	glBufferData ( GL_ARRAY_BUFFER, ( 2 * iVertices ) * sizeof ( float ), texCoords, GL_STATIC_DRAW );
	glVertexAttribPointer (( GLuint ) 2, 2, GL_FLOAT, GL_FALSE, 0, (( GLubyte *) NULL + ( 0 )));
	glEnableVertexAttribArray ( 2 );

	glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, handle [ 3 ]);						// Elements
	glBufferData ( GL_ELEMENT_ARRAY_BUFFER, 6 * faces * sizeof ( unsigned int ), elements, GL_STATIC_DRAW );
	
	delete [] vertices;
	delete [] normals;
	delete [] texCoords;
	delete [] elements;

	glBindVertexArray ( 0 );
}

void VBOTeapot::GeneratePatches ( float* _vertices, float* _normals, float* _texCoords, unsigned int* _elements, int _grid )
{
	float* B = new float [ 4 * ( _grid + 1 )];			// Pre computed Bernstein basis functions
	float* dB = new float [ 4 * ( _grid + 1 )];			// Prec computed Bernstein derivitives of basis function

	int idx = 0, elIndex = 0, tcIndex = 0;

	// Pre-compute the basis functions ( Bernstein polynomials )
	// and derivates
	ComputeBasisFunctions ( B, dB, _grid );

	// Build each patch
	// The rim
	BuildPatchReflect ( 0, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	
	// The body
	BuildPatchReflect ( 1, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	BuildPatchReflect ( 2, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	
	// The lid
	BuildPatchReflect ( 3, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	BuildPatchReflect ( 4, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	
	// The bottom
	BuildPatchReflect ( 5, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, true, true );
	
	// The handle
	BuildPatchReflect ( 6, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, false, true );
	BuildPatchReflect ( 7, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, false, true );
	
	// The spout
	BuildPatchReflect ( 8, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, false, true );
	BuildPatchReflect ( 9, B, dB, _vertices, _normals, _texCoords, _elements, idx, elIndex, tcIndex, _grid, false, true );

	delete [] B;
	delete [] dB;
}

void VBOTeapot::MoveLid ( int _grid, float* _vertices, const mat4 &_lidTranform )
{
	int start = 3 * 12 * ( _grid + 1 ) * ( _grid + 1 );
	int end = 3 * 20 * ( _grid + 1 ) * ( _grid + 1 );

	for ( int i = start ; i < end ; i += 3 )
	{
		vec4 vertex = vec4 ( _vertices [ i ], _vertices [ i + 1 ], _vertices [ i + 2 ], 1.f );
		vertex = _lidTranform * vertex;
		_vertices [ i ] = vertex.x;
		_vertices [ i + 1 ] = vertex.y;
		_vertices [ i + 2 ] = vertex.z;
	}
}

void VBOTeapot::BuildPatchReflect ( int _patchNum, float* _B, float* _dB,
									float* _vertices, float* _normals, float* _texCoord, unsigned int* _elements,
									int &_index, int &_elementsIndex, int &_texCoordsIndex, int _grid,
									bool _reflectX, bool _reflectY )
{
	vec3 patch [ 4 ][ 4 ];
	vec3 patchRevV [ 4 ][ 4 ];

	getPatch ( _patchNum, patch, false );
	getPatch ( _patchNum, patchRevV, true );

	// Patch without modification
	BuildPatch ( patch, _B, _dB, _vertices, _normals, _texCoord, _elements, _index, _elementsIndex, _texCoordsIndex, _grid, mat3 ( 1.f ), true );

	// Patch reflected in x
	if ( _reflectX )
	{
		BuildPatch ( patchRevV, _B, _dB, _vertices, _normals, _texCoord, _elements, _index, _elementsIndex, _texCoordsIndex, _grid, mat3 (	vec3 ( -1.f, 0.f, 0.f ),
																																			vec3 ( 0.f, 1.f, 0.f ),
																																			vec3 ( 0.f, 0.f, 1.f )), false );
	}

	// Patch reflectef in y
	if ( _reflectY )
	{
		BuildPatch ( patchRevV, _B, _dB, _vertices, _normals, _texCoord, _elements, _index, _elementsIndex, _texCoordsIndex, _grid, mat3 (	vec3 ( 1.f, 0.f, 0.f ),
																																			vec3 ( 0.f, -1.f, 0.f ),
																																			vec3 ( 0.f, 0.f, 1.f )), false );
	}

	// Patch reflected in x and y
	if ( _reflectX && _reflectY )
	{
		BuildPatch ( patch, _B, _dB, _vertices, _normals, _texCoord, _elements, _index, _elementsIndex, _texCoordsIndex, _grid, mat3 (	vec3 ( -1.f, 0.f, 0.f ),
																																			vec3 ( 0.f, -1.f, 0.f ),
																																			vec3 ( 0.f, 0.f, 1.f )), true );
	}
}

void VBOTeapot::BuildPatch ( vec3 _patch [][ 4 ],
						float* _B, float* _dB,
						float* _vertices, float* _normals, float* _texCoord, unsigned int* _elements,
						int &_index, int &_elementsIndex, int &_texCoordsIndex, int _grid,
						mat3 _reflect, bool _invertNormal )
{
	int startIndex = _index / 3;
	float tcFactor = 1.f / _grid;

	for ( int i = 0 ; i <= _grid ; i ++ )
	{
		for ( int j = 0 ; j <= _grid ; j ++ )
		{
			vec3 pt = _reflect * Evaluate ( i, j, _B, _patch );
			vec3 norm = _reflect * EvaluateNormal ( i, j, _B, _dB, _patch );

			if ( _invertNormal )
			{
				norm = -norm;
			}

			_vertices [ _index ] = pt.x;
			_vertices [ _index + 1 ] = pt.y;
			_vertices [ _index + 2 ] = pt.z;

			_normals [ _index ] = norm.x;
			_normals [ _index + 1 ] = norm.y;
			_normals [ _index + 2 ] = norm.z;

			_texCoord [ _texCoordsIndex ] = i * tcFactor;
			_texCoord [ _texCoordsIndex + 1 ] = j * tcFactor;

			_index += 3;
			_texCoordsIndex += 2;
		}
	}

	for ( int i = 0 ; i < _grid ; i ++ )
	{
		int iStart = i * ( _grid + 1 ) + startIndex;
		int nextiStart = ( i + 1 ) * ( _grid + 1 ) + startIndex;

		for ( int j = 0 ; j < _grid ; j ++ )
		{
			_elements [ _elementsIndex ] = iStart + j;
			_elements [ _elementsIndex + 1 ] = nextiStart + j + 1;
			_elements [ _elementsIndex + 2 ] = nextiStart + j;

			_elements [ _elementsIndex + 3 ] = iStart + j;
			_elements [ _elementsIndex + 4 ] = iStart + j + 1;
			_elements [ _elementsIndex + 5 ] = nextiStart + j + 1;

			_elementsIndex += 6;
		}
	}
}

void VBOTeapot::getPatch ( int _patchNum, vec3 _patch [][ 4 ], bool _reverseV )
{
	for ( int u = 0 ; u < 4 ; u ++ )		// Loop in u direction
	{
		for ( int v = 0 ; v < 4 ; v ++ )	// Loop in v direction
		{
			if ( _reverseV )
			{
				_patch [ u ][ v ] = vec3 (	Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + ( 3 - v )]][ 0 ],
											Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + ( 3 - v )]][ 1 ],
											Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + ( 3 - v )]][ 2 ]);
			}

			else
			{
				_patch [ u ][ v ] = vec3 (	Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + v ]][ 0 ],
											Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + v ]][ 1 ],
											Teapot::cpdata [ Teapot::patchdata [ _patchNum ][ u * 4 + v ]][ 2 ]);
			}
		}
	}
}

void VBOTeapot::ComputeBasisFunctions ( float* _B, float* _dB, int _grid )
{
	float inc = 1.f / _grid;

	for ( int i = 0 ; i <= _grid ; i ++ )
	{
		float t = i * inc;
		float tSqr = t * t;
		float oneMinusT = ( 1.f - t );
		float oneMinusT2 = oneMinusT * oneMinusT;

		_B [ i * 4 + 0 ] = oneMinusT * oneMinusT2;
		_B [ i * 4 + 1 ] = 3.f * oneMinusT2 * t;
		_B [ i * 4 + 2 ] = 3.f * oneMinusT * tSqr;
		_B [ i * 4 + 3 ] = t * tSqr;

		_dB [ i * 4 + 0 ] = -3.f * oneMinusT2;
		_dB [ i * 4 + 1 ] = -6.f * t * oneMinusT + 3.f * oneMinusT2;
		_dB [ i * 4 + 2 ] = -3.f * tSqr + 6.f * t * oneMinusT;
		_dB [ i * 4 + 3 ] = 3.f * tSqr;
	}
}

vec3 VBOTeapot::Evaluate ( int _gridU, int _gridV, float* _B, vec3 _patch [][ 4 ])
{
	vec3 p ( 0.f, 0.f, 0.f );

	for ( int i = 0 ; i < 4 ; i ++ )
	{
		for ( int j = 0 ; j < 4 ; j ++ )
		{
			p += _patch [ i ][ j ] * _B [ _gridU * 4 + i ] * _B [ _gridV * 4 + j ];
		}
	}

	return p;
}

vec3 VBOTeapot::EvaluateNormal ( int _gridU, int _gridV, float* _B, float* _dB, vec3 _patch [][ 4 ])
{
	vec3 du ( 0.f, 0.f, 0.f );
	vec3 dv ( 0.f, 0.f, 0.f );

	for ( int i = 0 ; i < 4 ; i ++ )
	{
		for ( int j = 0 ; j < 4 ; j ++ )
		{
			du += _patch [ i ][ j ] * _dB [ _gridU * 4 + i ] * _B [ _gridV * 4 + j ];
			dv += _patch [ i ][ j ] * _B [ _gridU * 4 + i ] * _dB [ _gridV * 4 + j ];
		}
	}
	
	vec3 norm = glm::cross ( du, dv );

	if ( glm::length ( norm ) != 0.f )
	{
		norm = glm::normalize ( norm );
	}

	return norm;
}

void VBOTeapot::Render () const
{
	glBindVertexArray ( vaoHandle );
	glDrawElements ( GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, (( GLubyte *) NULL + ( 0 )));
}