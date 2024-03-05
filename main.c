#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Function to handle errors
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Guwno w dupie", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    // Print OpenGL version
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        float angle = (float)time * 50.0f; // Adjust the speed of rotation by changing the multiplier

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotate about the z-axis

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glBegin(GL_TRIANGLES);                              // Start Drawing A Triangle
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);                  // Top Of Triangle (Front)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f(-0.5f,-0.5f, 0.5f);                  // Left Of Triangle (Front)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f( 0.5f,-0.5f, 0.5f);                  // Right Of Triangle (Front)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);                  // Top Of Triangle (Right)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f( 0.5f,-0.5f, 0.5f);                  // Left Of Triangle (Right)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f( 0.5f,-0.5f, -0.5f);                 // Right Of Triangle (Right)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);                  // Top Of Triangle (Back)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f( 0.5f,-0.5f, -0.5f);                 // Left Of Triangle (Back)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f(-0.5f,-0.5f, -0.5f);                 // Right Of Triangle (Back)
        glColor3f(1.0f,0.0f,0.0f);                      // Red
        glVertex3f( 0.0f, 0.5f, 0.0f);                  // Top Of Triangle (Left)
        glColor3f(0.0f,0.0f,1.0f);                      // Blue
        glVertex3f(-0.5f,-0.5f,-0.5f);                  // Left Of Triangle (Left)
        glColor3f(0.0f,1.0f,0.0f);                      // Green
        glVertex3f(-0.5f,-0.5f, 0.5f);                  // Right Of Triangle (Left)
    glEnd();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();
    return 0;
}