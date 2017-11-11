#ifndef __SKYBOX_H_
#define __SKYBOX_H_

#include "GLBasics.h"

#include "Drawable.h"

class SkyBox : public Drawable
{

public:

	SkyBox ();

	void Render () const;


private:

	GLuint vaoHandle;

};

#endif