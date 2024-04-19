#include <GL/glut.h>
#include <SOIL/SOIL.h> // Make sure to include SOIL for image loading

GLuint texture; // Texture ID

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Draw a rectangle to fill the entire window with the texture
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(0, 0);
        glTexCoord2f(1, 0); glVertex2f(glutGet(GLUT_WINDOW_WIDTH), 0);
        glTexCoord2f(1, 1); glVertex2f(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        glTexCoord2f(0, 1); glVertex2f(0, glutGet(GLUT_WINDOW_HEIGHT));
    glEnd();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    // Load texture
    texture = SOIL_load_OGL_texture(
        "", // Path to your image
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y // Depending on the orientation of your texture
    );

    // Check for errors
    // if (texture == 0) {
    //     printf("Error loading texture: %s\n", SOIL_last_result());
    //     exit(1); // Exit the program with error status
    // }

    // Set texture parameters
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int main(int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Set display mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // Set window size and position
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);

    // Create the window
    glutCreateWindow("OpenGL Window with Background Texture");

    // Set display and reshape callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Initialize
    init();

    // Start the main loop
    glutMainLoop();

    return 0;
}
