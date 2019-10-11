#version 430 core

// Inputs and outputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 colour;

uniform layout (location = 2) mat4 MVP_matrix;

out flat vec4 vertexColour;




void main()
{
    //vec3 mirroredPosition = vec3(-position.x, -position.y, position.z);
    vertexColour = vec4(colour);

    gl_Position = MVP_matrix * vec4(position, 1.0f);
}
