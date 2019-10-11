#version 430 core

// Inputs and outputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;
layout (location = 2) in vec3 normal;

uniform layout (location = 3) mat4 MVP_matrix;
uniform layout (location = 4) mat4 M_matrix;

out vec4 vertexColour;
out vec3 normals;

void main()
{
    vertexColour = vec4(colour);

    normals = normalize(mat3(M_matrix) * normal);

    gl_Position = MVP_matrix * vec4(position, 1.0f);
}
