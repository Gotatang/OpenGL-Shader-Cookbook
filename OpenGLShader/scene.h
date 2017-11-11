#ifndef __SCENE_H_
#define __SCENE_H_

class Scene
{

public:

	Scene() : bAnimate ( true ) {};

	virtual void Init () = 0;
	virtual void Update ( float t ) = 0;
	virtual void Render () = 0;

	void animate ( bool _value ) { bAnimate = _value; };
	bool animating () { return bAnimate; };

protected:
	
	bool bAnimate;
};

#endif