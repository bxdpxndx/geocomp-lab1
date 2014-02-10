#include "octree.h"
#include "ase.h"
#include <cassert>
#include <cmath>
#include <cfloat>
#define EPSILON 0.00001

inline bool float_is_zero(float f) {
    return fabs(f) < EPSILON;
}


Octree::Octree(const CASEModel &m) : triangles(m.getTriangles()), model(m){
	max_depth = 3;
	min_tri = 10;
    root_node = new OctreeNode(this, max_depth, min_tri);
    for(unsigned i = 0; i < triangles.size(); i++) {
        root_node->add_triangle(i);
    }
    root_node->set_BBox(m.get_BBox());
    root_node->build_octree();

}

vector3f Octree::get_vertex(unsigned idx) const{
    return model.get_vertex(idx);
}

void Octree::render() const {
    root_node->render();
}

const triangle* Octree::get_intersecting_triangle(const vector3f & point, const vector3f & direction, float *distance) const {
    float f = FLT_MAX;
    if (root_node->get_BBox().intersects(point,direction)) {
        int t = root_node->get_intersecting_triangle(point, direction, &f);
        if (t != -1) {
        if (distance != NULL){
            *distance = f;
        }
            return &triangles[t];
        }
    }
    return NULL;
}

Octree::~Octree() {
    delete root_node;
}

OctreeNode::OctreeNode(const Octree *r, int d, int min_t) : root(r), depth(d), min_tri(min_t), n_tri(0) {
    for(int i = 0; i < 8; i++) {
        children[i] = NULL;
    }
}
void OctreeNode::add_triangle(int idx) {
    if (triangle_references.empty() || triangle_references.back() != idx) {
        n_tri ++;
        triangle_references.push_back(idx);
    }
}

int OctreeNode::get_intersecting_triangle(const vector3f & point, const vector3f & direction, float * best_distance) const {
    
    int best_triangle = -1;
    float distance;
    // if there are any triangles, examine them
    if (! triangle_references.empty()){

        for (std::vector<int>::const_iterator it = triangle_references.begin(); it < triangle_references.end(); it ++) {
            triangle t = root->triangles[*it];
            if (triangle_intersects(root->get_vertex(t.a), root->get_vertex(t.b), root->get_vertex(t.c), point, direction, &distance)) {
                if(distance < *best_distance) { 
                    // pointer fun
                    best_triangle = *it;
                    *best_distance = distance;
                }
            }
        }
    }
    // else proceed recursively
    else {
        for (int i = 0; i < 8; ++i) {
            if (children[i] != NULL && children[i]->get_BBox().intersects(point, direction)) {
                int t = children[i]->get_intersecting_triangle(point, direction, &distance);
                if(t != -1 && distance < *best_distance) { 
                    best_triangle = t;
                    *best_distance = distance;
                }
            }
        }
    }
    return best_triangle;
}

void OctreeNode::render() const {
    BBox.render();
    for(int i = 0; i < 8; i++) {
        if (children[i] != NULL) {
            children[i]->render();
        }
    }
}

OctreeNode::~OctreeNode() {
    for(int i = 0; i < 8; i++) {
        if (children[i] != NULL) {
            delete children[i];
        }
    }
}

void OctreeNode::set_BBox(box3f b) {
    BBox = b;
}

const box3f & OctreeNode::get_BBox() const {
    return BBox;
}

void OctreeNode::build_octree() {
    if (n_tri < min_tri || depth == 0) {
        return;
    }
    vector3f center = BBox.get_center();
    for (int i = 0; i < 8; i++) {
        children[i] = new OctreeNode(root, depth-1, min_tri);
        box3f box(i&4?BBox.right:center.x,
                  i&2?BBox.top:center.y,
                  i&1?BBox.front:center.z,
                  i&4?center.x:BBox.left,
                  i&2?center.y:BBox.bottom,
                  i&1?center.z:BBox.back); 
        children[i]->set_BBox(box);

    }
    while(triangle_references.size() > 0) {
        int idx = triangle_references.back();
        triangle_references.pop_back();
        triangle t = root->triangles[idx];
        
        children[BBox.get_octant(root->get_vertex(t.a))]->add_triangle(idx);
        children[BBox.get_octant(root->get_vertex(t.b))]->add_triangle(idx);
        children[BBox.get_octant(root->get_vertex(t.c))]->add_triangle(idx);

    }
    for (int i = 0; i < 8; i++) {
        children[i]->build_octree();
    }
}

// from http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool triangle_intersects(const vector3f & v1,
                         const vector3f & v2,
                         const vector3f & v3,
                         const vector3f & point,
                         const vector3f & direction,
                         float * distance) {

    vector3f edge1 = v1 - v2;
    vector3f edge2 = v3 - v1;
    vector3f P = crossProduct(direction, edge2);
    float det = dotProduct(edge1, P);
    if(float_is_zero(det)) {
        // ray lies in plane of the triangle
        return false;
    }
    float inv_det = 1.0f / det;
    
    vector3f T = point - v1;
    float u = dotProduct(T,P) * inv_det;

    if (u < 0 || u > 1) {
        return false;
    }
    vector3f Q = crossProduct(T, edge1);
    float v = dotProduct(direction, Q) * inv_det;
    if(v < 0.f || u + v  > 1.f) {
        return false;
    }

    float t = dotProduct(edge2, Q) * inv_det;

    if (t > 0 && t < *distance) {
        // distance is positive 
        // (intersection doesn't happen behind the camera)
        *distance = t;
        return true;
    }

    return false;
}
