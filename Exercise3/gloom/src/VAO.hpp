#ifndef VAO_HPP
#define VAO_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <math.h>

// Local headers
#include "mesh.hpp"

#define DIM_COORDINATES 3
#define NUM_COLOURS 4


// Creates a Vertex Array Object
unsigned int createVAO(std::vector<float> vertexCoordinates, std::vector<unsigned int> indices, std::vector<float> colours, std::vector<float> normals);

// Creates VAO from Mesh
unsigned int createVAOfromMesh(Mesh mesh);


#endif
