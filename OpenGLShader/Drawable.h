#ifndef __DRAWABLE_H_
#define __DRAWABLE_H_

class Drawable
{

public:

	Drawable ();

	virtual void Render () const = 0;
};

#endif