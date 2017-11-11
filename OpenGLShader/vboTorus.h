#ifndef __VBOTORUS_H_
#define __VBOTORUS_H_

#include "Drawable.h"
#include "GLBasics.h"

class VBOTorus : public Drawable
{

public:

	VBOTorus ( float, float, int, int );

	void Render () const;

	int getVertexArrayHandle ();


private:

	unsigned int vaoHandle;
	int faces, rings, sides;

	void generateVerts ( float* _vertices, float* _normals, float* _textureCoord, unsigned int* _elements, float _innerRadius, float _outerRadius );
};

#endif