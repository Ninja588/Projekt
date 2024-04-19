#include <GL/glut.h>
#include <stdio.h>
#include <stdbool.h>

// Function prototypes
void display();
void mouse(int button, int state, int x, int y);
void drawButton(float x, float y, float width, float height, const char *label);
void handleButtonClick(int x, int y);
void init();

bool gameStarted = false;

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2048 Game Menu");

    // Set up OpenGL
    init();

    // Display function
    glutDisplayFunc(display);
    
    // Mouse function
    glutMouseFunc(mouse);

    // Main loop
    glutMainLoop();
    return 0;
}

// Display function
void display() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw buttons
    glColor3f(0.5, 0.5, 0.5);
    drawButton(200, 300, 100, 50, "Start");
    drawButton(200, 200, 100, 50, "Credits");
    drawButton(200, 100, 100, 50, "Quit");

    // Flush buffer
    glFlush();
}

// Mouse function
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        handleButtonClick(x, 500 - y);
    }
}

// Draw a button with label
void drawButton(float x, float y, float width, float height, const char *label) {
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(x + width / 2 - 20, y + height / 2);
    while (*label) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *label++);
    }
}

// Handle button click events
void handleButtonClick(int x, int y) {
    if (x >= 200 && x <= 300 && y >= 300 && y <= 350) {
        printf("Start\n");
        gameStarted = true;
    } else if (x >= 200 && x <= 300 && y >= 200 && y <= 250) {
        printf("Credits:\n");
        printf("Gra stworzona przez:\nKamil Derszniak\nDawid Janowski\nFilip Dorowski\n");
    } else if (x >= 200 && x <= 300 && y >= 100 && y <= 150) {
        exit(0);
    }
}

// Initialization function
void init() {
    // Set clear color to white
    glClearColor(1.0, 1.0, 1.0, 1.0);

    // Set up the coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);
    glMatrixMode(GL_MODELVIEW);
}
