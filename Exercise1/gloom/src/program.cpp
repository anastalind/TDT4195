// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

/* Creates a Vertex Array Object containting triangles */
unsigned int createVAO(std::vector<float> vertexCoordinates, std::vector<unsigned int> indices) {

    // Generating a single Vertex Array Object (VAO) and binding it
    unsigned int vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Generating a Vertex Buffer Object (VBO) and binding it
    unsigned int vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);

    // Transferring data from input vertexCoordinates to GPU
    glBufferData(GL_ARRAY_BUFFER, vertexCoordinates.size() * sizeof(float), &vertexCoordinates[0], GL_STATIC_DRAW);

    // Set the Vertex Attribute Pointer to specify the format of the VBO
    glVertexAttribPointer(
        0,
        DIM_COORDINATES, // Dimension of the triangle
        GL_FLOAT, // Data type of the values in the VBO
        GL_FALSE, // If OpenGL should normalise the values in the VBO
        0, // Defines the number of bytes between each entry in the buffer; 0 means OpenGL will deduce this for you
        nullptr // Defines the number of bytes until the first value in the VBO
    );

    // Enables the VBOs that should serve as inputs to the rendering pipeline
    glEnableVertexAttribArray(0);

    // Creates the index buffer which specifies how the vertices should be combined into primitives
    unsigned int indexBufferID = 0;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    return vertexArrayID;
}


void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.0f, 0.5f, 0.5f, 0.8f);

    // Creating two shaders: A vertex shader and a fragment shader
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");

    // Defining five distinct triangles
    std::vector <float> vertexCoordinates {-0.1, -0.1, 0.0, 0.1, -0.1, 0.0, 0.0, 0.1, 0.0,
                                           -0.6, -0.6, 0.0, 0.0, -0.6, 0.0, -0.6, 0.0, 0.0,
                                            0.3, -0.4, 0.0, 1.0, -0.4, 0.0, 0.6, 0.3, 0.0,
                                            0.5, 0.7, 0.0, 0.8, 0.9, 0.0, 0.4, 0.9, 0.0,
                                           -0.2, 0.3, 0.0, 0.2, 0.3, 0.0, 0.0, 0.9, 0.0};




    // Creating vector with the number of vertices to be drawn
    std::vector <unsigned int> indices;

    for (unsigned int i = 0; i * DIM_COORDINATES < vertexCoordinates.size(); i++) {indices.push_back(i);};

    unsigned int VAO = createVAO(vertexCoordinates, indices);

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program
        shader.activate();

        // Perform draw calls using, for example, glDrawArrays
        glBindVertexArray(VAO);
        printGLError();

        unsigned int count = vertexCoordinates.size()/DIM_COORDINATES;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        // Deactivate shader program
        shader.deactivate();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);

    }
    // Destroy shader
    shader.destroy();
}


void handleKeyboardInput(GLFWwindow* window)
{
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
