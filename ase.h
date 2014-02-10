#ifndef _ASE_LOADER_
#define _ASE_LOADER_

#include <iostream>
#include <vector>

#include "vector3f.h"
#include "box3f.h"

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
class triangle
{
public:
	union
	{
		struct{unsigned int t[3];};
		struct{unsigned int a,b,c;};
	};
};

/////////////////////////////////////////////////////////////////////////////////////////
// Simple ASE File format Loader
/////////////////////////////////////////////////////////////////////////////////////////
class CASEModel
{
public:
	CASEModel();
    const std::vector<triangle> & getTriangles() const;
    const vector3f get_vertex(unsigned int i) const;

public:
	// primary load model method
	bool load(const char* filename);

	// gl render
	void render() const;

    box3f get_BBox() const;

protected:
	std::vector<vector3f> m_vertices;
	std::vector<triangle> m_triangles;
};


#endif
