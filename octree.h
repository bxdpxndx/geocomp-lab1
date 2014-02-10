#pragma once
#include <vector>
#include "ase.h"
#include "box3f.h"

class OctreeNode;

class Octree {
public:
    const std::vector<triangle> &triangles;
    Octree(const CASEModel & m);
    ~Octree();
    
    vector3f get_vertex(unsigned int i) const;

    const triangle* get_intersecting_triangle(const vector3f & point, const vector3f & direction, float *distance) const;
    void render() const;
    int max_depth;
    int min_tri;

private:
    OctreeNode *root_node;
    CASEModel model;
};

class OctreeNode {
public:
	OctreeNode(const Octree *r, int d, int t);
	~OctreeNode();

    int get_intersecting_triangle(const vector3f & point, const vector3f & direction, float *distance) const;
    const Octree* root;
    void add_triangle(int idx);
	void build_octree();
    void set_BBox(box3f b);
    const box3f & get_BBox() const;
    void render() const;
private:
    box3f BBox;
    int depth;
	int min_tri;
    int n_tri; 
	std::vector<int> triangle_references;
	OctreeNode* children[8];
	OctreeNode* parent;

};

bool triangle_intersects(const vector3f & v1,
                         const vector3f & v2,
                         const vector3f & v3,
                         const vector3f & point,
                         const vector3f & direction,
                         float * best);
