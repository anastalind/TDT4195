#version 430 core

//in vec4 gl_FragCoord;
in flat vec4 vertexColour;

out vec4 fragColour;

void main()
{
    //color = vec4(sin(gl_FragCoord.x*0.5)*0.25 + 0.5, cos(gl_FragCoord.y*0.5)*0.25 + 0.5, 0.5f, 1.0f);
    fragColour = vertexColour;

}
