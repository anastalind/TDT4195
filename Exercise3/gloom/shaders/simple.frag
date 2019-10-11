#version 430 core

//in vec4 gl_FragCoord;
in vec4 vertexColour;
in vec3 normals;

out vec4 fragColour;

vec3 lightDirection = normalize(vec3(0.8, -0.5, 0.6));


void main()
{
    // Lambertian lighting model
    fragColour = vec4(vertexColour.rgb * max(0, dot(normals, -lightDirection)), vertexColour.a);

}
