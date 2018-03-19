/**
   Triangle - a storage class for a triangle face from a .obj file
 */

#include "Triangle.h"

int Triangle::init(unsigned int a, unsigned int b, unsigned int c,    
                   unsigned int an, unsigned int bn, unsigned int cn) {
  std::vector<unsigned int> v, vn;

  // vertex indices
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  // vertex normal indices
  vn.push_back(an);
  vn.push_back(bn);
  vn.push_back(cn);

  this->vertices = v;
  this->vertexNormals = vn;

  return 0;
}

Triangle::Triangle() {
  init(0,0,0,0,0,0);
}

Triangle::Triangle(unsigned int a, unsigned int b, unsigned int c,    
                   unsigned int an, unsigned int bn, unsigned int cn) {
  init(a,b,c,an,bn,cn);
}

Triangle::Triangle(unsigned int a, unsigned int b, unsigned int c) {
  init(a,b,c,0,0,0);
}

std::vector<unsigned int> Triangle::getVertices() const {
  return this->vertices;
}

std::vector<unsigned int> Triangle::getVertexNormals() const {
  return this->vertexNormals;
}

