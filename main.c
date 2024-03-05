#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Function to handle errors
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

int main() {
    // GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // set GLFW error callback
    glfwSetErrorCallback(error_callback);

    // create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Guwno w dupie", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // glew
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    // opengl version
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        double time = glfwGetTime();
        float angle = (float)time * 50.0f; // rotation speed

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(angle, 0.0f, 1.0f, 0.0f); // rotate about the y-axis

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

        // swap front and back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // terminate GLFW
    glfwTerminate();
    return 0;
}
