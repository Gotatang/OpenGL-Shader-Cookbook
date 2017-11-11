#ifndef __VBOMESH_H_
#define __VBOMESH_H_

#include "GLBasics.h"

#include "Drawable.h"

#include "glm\glm.hpp"
using glm::vec2;
using glm::vec3;
using glm::vec4;

#include <vector>
using std::vector;

#include <string>
using std::string;

class VBOMesh : public Drawable
{

public:

	VBOMesh ( const char* _filename, bool _reCenterMesh = false, bool _loadTex = false, bool _genTangents = false );

	void Render () const;

	void loadObj ( const char* _filename );

private:

	GLuint vaoHandle;
	GLuint faces;

	bool reCenterMesh, loadTex, genTangents;

	void trimString ( string &_str );
	void storeVBO ( const vector <vec3> &_points, const vector <vec3> &_normals, const vector <vec2> &_texCoords, const vector <GLuint> &_elements, const vector <vec4> &_tangents );
	void generateAveragedNormals ( const vector <vec3> &_points, vector <vec3> &_normals, const vector <GLuint> &_elements );
	void generateTangents ( const vector <vec3> &_points, const vector <vec3> &_normals, const vector <vec2> &_texCoords, const vector <GLuint> &_elements, vector <vec4> &_tangents );
	void center ( vector <vec3> &_vertices );
};

#endif