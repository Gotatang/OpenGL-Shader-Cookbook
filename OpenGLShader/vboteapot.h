#ifndef __VBOTEAPOT_H_
#define __VBOTEAPOT_H_

#include "Drawable.h"

#include "GLBasics.h"

#include "glm\glm.hpp"
using glm::vec3;
using glm::mat3;
using glm::mat4;


class VBOTeapot : public Drawable
{

public:

	VBOTeapot ( int _grid, const mat4& _lidTransform );
	
	void Render () const;

private :

	unsigned int vaoHandle;
	unsigned int faces;

	void GeneratePatches ( float* _vertices, float* _normals, float* _texCoords, unsigned int* _elements, int _grid );

	void MoveLid ( int _grid, float* _vertices, const mat4& _lidTransform );

	void ComputeBasisFunctions ( float* _B, float* _dB, int _grid );

	void BuildPatchReflect (	int _patchNum, float* _B, float* _dB,
								float* _vertices, float* _normals, float* _texCoord, unsigned int* _elements,
								int &_index, int &_elementsIndex, int &_texCoordsIndex, int _grid,
								bool _reflectX, bool _reflectY );

	void BuildPatch (	vec3 _patch [][ 4 ],
						float* _B, float* _dB,
						float* _vertices, float* _normals, float* _texCoord, unsigned int* _elements,
						int &_index, int &_elementsIndex, int &_texCoordsIndex, int _grid,
						mat3 _reflect, bool _invertNormal );

	void getPatch ( int _patchNum, vec3 _patch [][ 4 ], bool reverseV );

	vec3 Evaluate ( int _gridU, int _gridV, float* _B, vec3 _patch [][ 4 ]);
	vec3 EvaluateNormal ( int _gridU, int _gridV, float* _B, float* _dB, vec3 _patch [][ 4 ]);
};

#endif