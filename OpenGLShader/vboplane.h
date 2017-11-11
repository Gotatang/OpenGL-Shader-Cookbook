#ifndef __VBOPLANE_H_
#define __VBOPLANE_H_

#include "Drawable.h"

class VBOPlane : public Drawable
{

public:

	VBOPlane ( float _xSize, float _zSize, int _xDivs, int _zDivs, float _sMax = 1.f, float _tMax = 1.f );

	void Render () const;

private:

	unsigned int vaoHandle;
	int faces;

};

#endif