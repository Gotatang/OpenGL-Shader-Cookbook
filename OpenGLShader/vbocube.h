#ifndef __VBOCUBE_H_
#define __VBOCUBE_H_

#include "GLBasics.h"
#include "Drawable.h"

class VBOCube : public Drawable
{

public:

	VBOCube ();

	void Render () const;


private:

	unsigned int vaoHandle;
};

#endif