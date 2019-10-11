// Local headers
#include "program.hpp"

// A vector that stores the value of translation motions along the x, y and z axes
glm::vec3 camera_position = glm::vec3(0.0f, -25.0f, -80.0f); // Starting in the visible interval (z in [-100.0, -1.0])

// Variables that store the value of rotational motions
float x_angle = 0.5f;
float y_angle = 0.0f;
float z_angle = 0.0f;

// Vectors that represent the xyz axes
glm::vec3 x_rotation_axis = glm::vec3(1.0f, 0.0f, 0.0f);
glm::vec3 y_rotation_axis = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 z_rotation_axis = glm::vec3(0.0f, 0.0f, 1.0f);

// Enum that represent the axes
typedef enum {X_AXIS, Y_AXIS, Z_AXIS} rotation_axis;

// Number of helicopters to be animated
#define NUM_HELICOPTERS 5

// Speed of helicopter rotors
#define ROTOR_SPEED 10.0f

// Offset for drawing multiple helicopters without crashes
#define HELICOPTER_TIME_OFFSET 0.8f

/* Constructs and returns a scene graph */
SceneNode* init_scene_graph() {
    // Scene nodes
    SceneNode* root_node;

    SceneNode* terrain_node;

    // Loading the lunar surface mesh
    Mesh lunar_terrain("Terrain mesh");
    lunar_terrain = loadTerrainMesh("../gloom/resources/lunarsurface.obj");

    root_node = createSceneNode();
    terrain_node = createSceneNode();

    addChild(root_node, terrain_node);
    terrain_node->vertexArrayObjectID = createVAOfromMesh(lunar_terrain);
    terrain_node->VAOIndexCount = lunar_terrain.indices.size();

    for (int i = 0; i < NUM_HELICOPTERS; i++) {
        // Loading the helicopter
        struct Helicopter helicopter;
        helicopter = loadHelicopterModel("../gloom/resources/helicopter.obj");

        // Generate one Scene Node for each object
        SceneNode* body_node = createSceneNode();
        SceneNode* mainRotor_node = createSceneNode();
        SceneNode* tailRotor_node = createSceneNode();
        SceneNode* door_node = createSceneNode();

        // Organise the objects into a Scene Graph
        addChild(terrain_node, body_node);

        addChild(body_node, mainRotor_node);
        addChild(body_node, tailRotor_node);
        addChild(body_node, door_node);

        // Initialize values in the SceneNode data structure
        // VAOs

        body_node->vertexArrayObjectID = createVAOfromMesh(helicopter.body);
        mainRotor_node->vertexArrayObjectID = createVAOfromMesh(helicopter.mainRotor);
        tailRotor_node->vertexArrayObjectID = createVAOfromMesh(helicopter.tailRotor);
        door_node->vertexArrayObjectID = createVAOfromMesh(helicopter.door);

        // VAO indices

        body_node->VAOIndexCount = (helicopter.body).indices.size();
        mainRotor_node->VAOIndexCount = (helicopter.mainRotor).indices.size();
        tailRotor_node->VAOIndexCount = (helicopter.tailRotor).indices.size();
        door_node->VAOIndexCount = (helicopter.door).indices.size();

        // Reference points
        tailRotor_node->referencePoint = glm::vec3(0.35f, 2.30f, 10.40f);

        // Animated parts
        (body_node->animated_Y).push_back(mainRotor_node);
        (body_node->animated_X).push_back(tailRotor_node);
    }

    return root_node;
}


/* Updates view matrix */
glm::mat4x4 update_view_matrix(glm::mat4x4 translation_matrix, glm::mat4x4 rotation_X_matrix, glm::mat4x4 rotation_Y_matrix) {

    translation_matrix = glm::translate(camera_position);
    rotation_X_matrix = glm::rotate(x_angle, x_rotation_axis);
    rotation_Y_matrix = glm::rotate(y_angle, y_rotation_axis);

    return rotation_X_matrix * rotation_Y_matrix  * translation_matrix;
}

/* Updates MVP matrix and draws scene node */
void draw_scene_node(SceneNode* node, glm::mat4 view_projection_matrix) {
    glm::mat4x4 MVP_matrix = view_projection_matrix * node->currentTransformationMatrix;

    glUniformMatrix4fv(3, 1, GL_FALSE, &MVP_matrix[0][0]);
    glUniformMatrix4fv(4, 1, GL_FALSE, &node->currentTransformationMatrix[0][0]);

    glBindVertexArray(node->vertexArrayObjectID);

    glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);

    for(SceneNode* child : node->children) {
        draw_scene_node(child, view_projection_matrix);
    }

}

/* Updates the translation matrix by position*/
glm::mat4 update_translation_matrix(glm::vec3 position) {
    return glm::translate(position);
}

/* Updates rotation matrix by translating object to the origin, perform rotation about three axes and translating back */
glm::mat4 update_rotation_matrix(glm::vec3 rotation, glm::vec3 reference) {
    glm::mat4x4 translate_to_origin = glm::translate(-reference);

    glm::mat4x4 rotation_x = glm::rotate(rotation.x, x_rotation_axis);
    glm::mat4x4 rotation_y = glm::rotate(rotation.y, y_rotation_axis);
    glm::mat4x4 rotation_z = glm::rotate(rotation.z, z_rotation_axis);

    glm::mat4x4 translate_from_origin = glm::translate(reference);

    return (translate_from_origin * rotation_x * rotation_y * rotation_z * translate_to_origin);
}


/* Updates scene node by setting a nodes model matrix to updated translation and rotation matrices and the parent's transformation matrix */
void update_scene_node(SceneNode* node, glm::mat4 parent_transformation) {
    glm::mat4x4 node_translation = update_translation_matrix(node->position);
    glm::mat4x4 node_rotation = update_rotation_matrix(node->rotation, node->referencePoint);

    node->currentTransformationMatrix = parent_transformation * node_translation * node_rotation;

    for(SceneNode* child : node->children){
        update_scene_node(child, node->currentTransformationMatrix);
    }
}

/* Spins one helicopter part (node) about axis based on time_elapsed as to appear smooth */
void spin_helicopter_part(SceneNode* node, float speed, double time_elapsed, rotation_axis axis) {
    float step = speed * static_cast<float>(time_elapsed);

    switch(axis) {
        case X_AXIS:
            node->rotation.x += step;
            break;

        case Y_AXIS:
            node->rotation.y += step;
            break;

        case Z_AXIS:
            node->rotation.z += step;
            break;

        default:
            break;
    }
}

/* Rotates all helicopter parts that are placed in node's animated vectors, which denote which of its children should rotate about which axis */
void rotate_animated_parts(SceneNode* node, float speed, double time_elapsed) {
    // Update rotation vector of parts that rotate around X axis
    for(SceneNode* animation : node->animated_X){
        spin_helicopter_part(animation, speed, time_elapsed, X_AXIS);
    }

    // Update rotation vector of parts that rotate around Y axis
    for(SceneNode* animation : node->animated_Y){
        spin_helicopter_part(animation, speed, time_elapsed, Y_AXIS);
    }

    // Update rotation vector of parts that rotate around Z axis
    for(SceneNode* animation : node->animated_Z){
        spin_helicopter_part(animation, speed, time_elapsed, Z_AXIS);
    }
}

/* Makes node follow a path given by heading by updating the nodes position and rotation variables */
void follow_path(SceneNode* node, double current_time) {
    struct Heading heading = simpleHeadingAnimation(current_time);
    node->position.x = heading.x;
    node->position.z = heading.z;

    node->rotation = glm::vec3(heading.pitch, heading.yaw, heading.roll);
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
    glClearColor(0.0f, 0.3f, 0.7f, 1.0f);

    // Creating two shaders: A vertex shader and a fragment shader
    Gloom::Shader shader;
    shader.makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag");

    /* Task 4 */
    // Creates the Model - View - Projection matrix and initializes it to a 4x4 identity matrix
    glm::mat4x4 VP_matrix = glm::mat4(1.0f);

    // Transformation matrices
    glm::mat4x4 translation_matrix = glm::translate(camera_position);
    glm::mat4x4 rotation_X_matrix = glm::rotate(x_angle, x_rotation_axis);
    glm::mat4x4 rotation_Y_matrix = glm::rotate(y_angle, y_rotation_axis);

    // View matrix
    glm::mat4x4 view_matrix = rotation_X_matrix * rotation_Y_matrix * translation_matrix;

    // Parameters of the projection matrix
    float FOVRadians = glm::radians(70.0); // Field of view
    float aspect_ratio = windowWidth/windowHeight;
    float near_plane = 1.0f; // As given in the assignment
    float far_plane = 10000.0f; // As given in the assignment

    // Projection matrix
    glm::mat4x4 projection_matrix = glm::perspective(FOVRadians, aspect_ratio, near_plane, far_plane);

    SceneNode* root = init_scene_graph();
    SceneNode* terrain = root->children[0];

    double current_time = 0.00;

    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader program
        shader.activate();

        // Time elapsed since last time function was called
        double time_elapsed = getTimeDeltaSeconds();
        current_time += time_elapsed;

        double current_time_with_offset = current_time;

        // Rotate all parts that are animated
        for (SceneNode* child : terrain->children) {
            rotate_animated_parts(child, ROTOR_SPEED, time_elapsed);
            follow_path(child, current_time_with_offset);
            current_time_with_offset += HELICOPTER_TIME_OFFSET;
        }

        // Update view projection matrix
        view_matrix = update_view_matrix(translation_matrix, rotation_X_matrix, rotation_Y_matrix);
        VP_matrix = projection_matrix * view_matrix;

        // Update and draw all scene nodes
        update_scene_node(root, glm::mat4(1.0f));
        draw_scene_node(root, VP_matrix);

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
    float speed = 0.05;
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // x translation
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_position.x += speed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_position.x -= speed;
    }

    // y translation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_position.y -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
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
        x_angle -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        x_angle += speed;
    }

    // yaw
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        y_angle -= speed;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        y_angle += speed;
    }

}
