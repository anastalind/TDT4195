#include "VAO.hpp"

/* Creates a Vertex Array Object containing triangles */
unsigned int createVAO(std::vector<float> vertexCoordinates, std::vector<unsigned int> indices,
                       std::vector<float> colours, std::vector<float> normals) {

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

    // Transferring data from input colours to GPU
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

    // Creates normal buffer
    unsigned int normalBufferID = 0;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);

    // Transferring data from input normals to GPU
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

    // Set the Vertex Attribute Pointer to specify the format of the VBO
    glVertexAttribPointer(
        2,
        DIM_COORDINATES, // x, y, z
        GL_FLOAT, // Data type of the values in the VBO
        GL_FALSE, // If OpenGL should normalise the values in the VBO
        0, // Defines the number of bytes between each entry in the buffer; 0 means OpenGL will deduce this for you
        nullptr // Defines the number of bytes until the first value in the VBO
    );

    // Enables the colour buffer
    glEnableVertexAttribArray(2);

    return vertexArrayID;
}
unsigned int createVAOfromMesh(Mesh mesh) {
    return createVAO(mesh.vertices, mesh.indices, mesh.colours, mesh.normals);
}
