/**
   Triangle - a storage class for a triangle face from a .obj file
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>


class Triangle {
 public:
  Triangle();
  Triangle(unsigned int a, unsigned int b, unsigned int c,    
           unsigned int an, unsigned int bn, unsigned int cn);
  Triangle(unsigned int a, unsigned int b, unsigned int c);

  std::vector<unsigned int> getVertices() const;
  std::vector<unsigned int> getVertexNormals() const;

 private:
  std::vector<unsigned int> vertices, vertexNormals; 
  
  int init(unsigned int a, unsigned int b, unsigned int c,    
           unsigned int an, unsigned int bn, unsigned int cn);
};

#endif
