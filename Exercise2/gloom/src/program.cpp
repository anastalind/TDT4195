// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

// A vector that stores the value of translation motions along the x, y and z axes
glm::vec3 camera_position = glm::vec3(0.0f, 0.0f, -3.0f); // Starting in the visible interval (z in [-100.0, -1.0])

// Variables that store the value of rotational motions (yaw and pitch)
float pitch_angle = 0.0;
float yaw_angle = 0.0;

// Vectors that represent the pitch and yaw axes
glm::vec3 pitch_rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 yaw_rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);


/* Creates a Vertex Array Object containing triangles */
unsigned int createVAO(std::vector<float> vertexCoordinates, std::vector<unsigned int> indices,
                       std::vector<float> colours)
{

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

    // Set the Vertex Attribute Pointer to specify the format of the VBO for colouring
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

    // Creates colour buffer
    unsigned int colourBufferID = 0;
    glGenBuffers(1, &colourBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colourBufferID);

    // Transferring data from input vertexCoordinates to GPU
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(float), &colours[0], GL_STATIC_DRAW);

    // Set the Vertex Attribute Pointer to specify the format of the VBO
    glVertexAttribPointer(
        1,
        NUM_COLOURS, // RGBA
        GL_FLOAT, // Data type of the values in the VBO
        GL_FALSE, // If OpenGL should normalise the values in the VBO
        0, // Defines the number of bytes between each entry in the buffer; 0 means OpenGL will deduce this for you
        nullptr // Defines the number of bytes until the first value in the VBO
    );

    // Enables the colour buffer
    glEnableVertexAttribArray(1);

    return vertexArrayID;
}

glm::mat4x4 updateview_matrix(glm::mat4x4 translation_matrix, glm::mat4x4 rotation_pitch_matrix, glm::mat4x4 rotation_yaw_matrix) {

    translation_matrix = glm::translate(camera_position);
    rotation_pitch_matrix = glm::rotate(pitch_angle, pitch_rotation_axis);
    rotation_yaw_matrix = glm::rotate(yaw_angle, yaw_rotation_axis);

    return rotation_yaw_matrix * rotation_pitch_matrix * translation_matrix;
}


void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default colour after clearing the colour buffer
    glClearColor(1.0f, 0.9f, 0.8f, 1.0f);

    // Creating two shaders: A vertex shader and a fragment shader
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");

    // Defining five distinct triangles
    std::vector <float> vertexCoordinates {
                                            0.1, -0.4, 0.0, 0.8, -0.4, 0.0, 0.6, 0.3, 0.0,
                                            0.6, 0.6, 0.2, 0.0, -0.6, 0.2, 0.6, -0.6, 0.2,
                                            -0.5, -0.5, 0.7, 0.5, -0.5, 0.7, 0.0, 0.5, 0.7
                                            };




    // Creating vector with the number of vertices to be drawn
    std::vector <unsigned int> indices;

    for (unsigned int i = 0; i * DIM_COORDINATES < vertexCoordinates.size(); i++) {indices.push_back(i);};

    // Setting colours of each of the vertices of the triangles
    std::vector <float> colours {
                                 1.0f, 0.0f, 0.0f, 0.8f, 0.5f, 0.2f, 0.9f, 0.8f, 0.1f, 0.8f, 0.0f, 0.8f,
                                 0.9f, 0.5f, 0.0f, 0.7f, 0.1f, 0.0f, 0.7f, 0.7f, 0.6f, 0.0f, 0.0f, 0.7f,
                                 0.0f, 0.3f, 0.0f, 0.5f, 0.9f, 0.3f, 0.0f, 0.5f, 0.0f, 0.3f, 0.9f, 0.5f};

    unsigned int VAO = createVAO(vertexCoordinates, indices, colours);

    /* Task 3 */
    /*
    glm::mat4x4 transformation_matrix = {{1.0, 0.0, 0.0, 0.0},
    {0.0, 1.0, 0.0, 0.0},
    {0.0, 0.0, 1.0, 0.0},
    {0.0, 0.0, 0.0, 1.0}};

    float inc_value = 0.0;
    float sin_value = sinf(inc_value)/2;
    */

    /* Task 4 */
    // Creates the Model - View - Projection matrix and initializes it to a 4x4 identity matrix
    glm::mat4x4 MVP_matrix = {{1.0, 0.0, 0.0, 0.0},
                              {0.0, 1.0, 0.0, 0.0},
                              {0.0, 0.0, 1.0, 0.0},
                              {0.0, 0.0, 0.0, 1.0}};

    // Model matrix
    glm::mat4x4 model_matrix = {{1.0, 0.0, 0.0, 0.0},
                                {0.0, 1.0, 0.0, 0.0},
                                {0.0, 0.0, 1.0, 0.0},
                                {0.0, 0.0, 0.0, 1.0}};

    // Transformation matrices
    glm::mat4x4 translation_matrix = glm::translate(camera_position);
    glm::mat4x4 rotation_pitch_matrix = glm::rotate(pitch_angle, pitch_rotation_axis);
    glm::mat4x4 rotation_yaw_matrix = glm::rotate(yaw_angle, yaw_rotation_axis);

    // View matrix
    glm::mat4x4 view_matrix = rotation_yaw_matrix * rotation_pitch_matrix * translation_matrix;

    // Parameters of the projection matrix
    float FOVRadians = glm::radians(70.0); // Field of view
    float aspect_ratio = windowWidth/windowHeight;
    float near_plane = 1.0; // As given in the assignment
    float far_plane = 100.0; // As given in the assignment

    // Projection matrix
    glm::mat4x4 projection_matrix = glm::perspective(FOVRadians, aspect_ratio, near_plane, far_plane);


    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program
        shader.activate();

        // Perform draw calls using, for example, glDrawArrays
        glBindVertexArray(VAO);

        /* Task 3 */
        /*
        inc_value += 0.01;
        sin_value = sinf(inc_value)/2;


        transformation_matrix = {{1.0, 0.0, 0.0, 0.0},
                                  {0.0, sin_value, 0.0, 0.0},
                                  {0.0, 0.0, 1.0, 0.0},
                                  {0.0, 0.0, 0.0, 1.0}};

        glUniformMatrix4fv(2, 1, GL_FALSE, &transformation_matrix[0][0]);

        unsigned int count = vertexCoordinates.size()/DIM_COORDINATES;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        */

        /* Task 4 */
        glUniformMatrix4fv(2, 1, GL_FALSE, &MVP_matrix[0][0]);

        unsigned int count = vertexCoordinates.size()/DIM_COORDINATES;
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

        view_matrix = updateview_matrix(translation_matrix, rotation_pitch_matrix, rotation_yaw_matrix);

        MVP_matrix = projection_matrix * view_matrix * model_matrix;
        /* */

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
    float speed = 0.01;
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // x translation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_position.x -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_position.x += speed;
    }

    // y translation
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_position.y -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_position.y += speed;
    }

    // z translation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera_position.z -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera_position.z += speed;
    }

    // pitch
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        pitch_angle += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        pitch_angle -= speed;
    }

    // yaw
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        yaw_angle += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        yaw_angle -= speed;
    }

}
