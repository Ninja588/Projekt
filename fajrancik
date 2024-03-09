#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Function to handle errors
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

struct posPoint
{
    float x;
    float y;
}vertexSquare[4];

void processInput(GLFWwindow *window, double time, double diff)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS)
    {
        /*
        diff = glfwGetTime()-time;
        time = glfwGetTime();
        glLoadIdentity();

        glTranslatef((float)diff/25.0,0.0f,0.0f);
        */
        glLoadIdentity();
        vertexSquare[0].x+=0.02f;
        vertexSquare[1].x+=0.02f;
        vertexSquare[2].x+=0.02f;
        vertexSquare[3].x+=0.02f;

        glBegin(GL_POLYGON);                              // Start Drawing A Triangle
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f( vertexSquare[0].x, vertexSquare[0].y );
        glVertex2f( vertexSquare[1].x, vertexSquare[1].y );
        glVertex2f( vertexSquare[2].x, vertexSquare[2].y );
        glVertex2f( vertexSquare[3].x, vertexSquare[3].y );
        glEnd();

    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS)
    {
        glLoadIdentity();
        vertexSquare[0].x-=0.02f;
        vertexSquare[1].x-=0.02f;
        vertexSquare[2].x-=0.02f;
        vertexSquare[3].x-=0.02f;

        glBegin(GL_POLYGON);                              // Start Drawing A Triangle
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f( vertexSquare[0].x, vertexSquare[0].y );
        glVertex2f( vertexSquare[1].x, vertexSquare[1].y );
        glVertex2f( vertexSquare[2].x, vertexSquare[2].y );
        glVertex2f( vertexSquare[3].x, vertexSquare[3].y );
        glEnd();
    }
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS)
    {
        /*
        diff = glfwGetTime()-time;
        time = glfwGetTime();
        glLoadIdentity();

        glTranslatef((float)diff/25.0,0.0f,0.0f);
        */
        glLoadIdentity();
        vertexSquare[0].y+=0.02f;
        vertexSquare[1].y+=0.02f;
        vertexSquare[2].y+=0.02f;
        vertexSquare[3].y+=0.02f;

        glBegin(GL_POLYGON);                              // Start Drawing A Triangle
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f( vertexSquare[0].x, vertexSquare[0].y );
        glVertex2f( vertexSquare[1].x, vertexSquare[1].y );
        glVertex2f( vertexSquare[2].x, vertexSquare[2].y );
        glVertex2f( vertexSquare[3].x, vertexSquare[3].y );
        glEnd();

    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS)
    {
        glLoadIdentity();
        vertexSquare[0].y-=0.02f;
        vertexSquare[1].y-=0.02f;
        vertexSquare[2].y-=0.02f;
        vertexSquare[3].y-=0.02f;

        glBegin(GL_POLYGON);                              // Start Drawing A Triangle
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f( vertexSquare[0].x, vertexSquare[0].y );
        glVertex2f( vertexSquare[1].x, vertexSquare[1].y );
        glVertex2f( vertexSquare[2].x, vertexSquare[2].y );
        glVertex2f( vertexSquare[3].x, vertexSquare[3].y );
        glEnd();
    }
    
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
    GLFWwindow* window = glfwCreateWindow(800, 800, "dupa w Guwnie", NULL, NULL);
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


    double time,diff;

        vertexSquare[0].x = 0.5f;
        vertexSquare[0].y = 0.5f;
        vertexSquare[1].x = 0.5f;
        vertexSquare[1].y = -0.5f;
        vertexSquare[2].x = -0.5f;
        vertexSquare[2].y = -0.5f;
        vertexSquare[3].x = -0.5f;
        vertexSquare[3].y = 0.5f;

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        
        processInput(window, time, diff);

        // Render here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double time = glfwGetTime();
        float angle = (float)time * 50.0f; // Adjust the speed of rotation by changing the multiplier

        glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
        
        //glRotatef(angle, 0.0f, 1.0f, 0.0f); // Rotate about the z-axis

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);



        glBegin(GL_POLYGON);                              // Start Drawing A Triangle
        glColor3f(1.0f,1.0f,1.0f);
        glVertex2f( vertexSquare[0].x, vertexSquare[0].y );
        glVertex2f( vertexSquare[1].x, vertexSquare[1].y );
        glVertex2f( vertexSquare[2].x, vertexSquare[2].y );
        glVertex2f( vertexSquare[3].x, vertexSquare[3].y );
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
