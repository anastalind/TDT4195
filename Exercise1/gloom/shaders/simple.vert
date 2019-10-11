#version 430 core

in vec3 position;

void main()
{
    vec3 mirroredPosition = vec3(-position.x, -position.y, position.z); //
    gl_Position = vec4(mirroredPosition, 1.0f);
}
