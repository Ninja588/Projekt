//#include <gl/glew.h>
//#include <GLFW/glfw3.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h> 
#include <stdlib.h>
#include <time.h>

#define SIZE 4

int grid[SIZE][SIZE];
int emptySpaces;

void updateEmptySpaces() {
    emptySpaces = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j] == 0) {
                emptySpaces++;
            }
        }
    }
}

void generateNewTile() {
    int value = rand() % 2 == 0 ? 2 : 4;
    if (emptySpaces > 0) {
        int randomIndex = rand() % emptySpaces;
        int count = 0;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (grid[i][j] == 0) {
                    if (count == randomIndex) {
                        grid[i][j] = value;
                        return;
                    }
                    count++;
                }
            }
        }
    }
    updateEmptySpaces();
}

void initializeGrid() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            grid[i][j] = 0;
        }
    }
    updateEmptySpaces();
    generateNewTile();
    generateNewTile();
}

void moveTilesLeft() {
    for (int i = 0; i < SIZE; ++i) {
        int lastMerged = -1;

        // od lewej do prawej
        for (int j = 1; j < SIZE; ++j) {
            if (grid[i][j] != 0) {
                int current = j;

                // pozycja do merge
                int mergePosition = current - 1;
                while (mergePosition >= 0 && grid[i][mergePosition] == 0) {
                    mergePosition--;
                }

                // merge
                if (mergePosition >= 0 && grid[i][current] == grid[i][mergePosition] && current - lastMerged <= SIZE) {
                    grid[i][mergePosition] *= 2;
                    grid[i][current] = 0;
                    lastMerged = mergePosition;
                } else {
                    // przesuwanie na lewo
                    if (mergePosition + 1 != current) {
                        grid[i][mergePosition + 1] = grid[i][current];
                        grid[i][current] = 0;
                    }
                }
            }
        }
    }
    generateNewTile();
    glutPostRedisplay();
}

void moveTilesRight() {
    for (int i = 0; i < SIZE; ++i) {
        int lastMerged = SIZE;

        for (int j = SIZE - 2; j >= 0; --j) {
            if (grid[i][j] != 0) {
                int current = j;

                int mergePosition = current + 1;
                while (mergePosition < SIZE && grid[i][mergePosition] == 0) {
                    mergePosition++;
                }

                if (mergePosition < SIZE && grid[i][current] == grid[i][mergePosition] && lastMerged - current <= SIZE) {
                    grid[i][mergePosition] *= 2;
                    grid[i][current] = 0;
                    lastMerged = mergePosition;
                } else {
                    if (mergePosition - 1 != current) {
                        grid[i][mergePosition - 1] = grid[i][current];
                        grid[i][current] = 0;
                    }
                }
            }
        }
    }
    generateNewTile();
    glutPostRedisplay();
}

void moveTilesUp() {
    for (int j = 0; j < SIZE; ++j) {
        int lastMerged = -1;

        for (int i = 1; i < SIZE; ++i) {
            if (grid[i][j] != 0) {
                int current = i;

                int mergePosition = current - 1;
                while (mergePosition >= 0 && grid[mergePosition][j] == 0) {
                    mergePosition--;
                }

                if (mergePosition >= 0 && grid[current][j] == grid[mergePosition][j] && current - lastMerged <= SIZE) {
                    grid[mergePosition][j] *= 2;
                    grid[current][j] = 0;
                    lastMerged = mergePosition;
                } else {
                    if (mergePosition + 1 != current) {
                        grid[mergePosition + 1][j] = grid[current][j];
                        grid[current][j] = 0;
                    }
                }
            }
        }
    }
    generateNewTile();
    glutPostRedisplay();
}

void moveTilesDown() {
    for (int j = 0; j < SIZE; ++j) {
        int lastMerged = SIZE;

        for (int i = SIZE - 2; i >= 0; --i) {
            if (grid[i][j] != 0) {
                int current = i;

                int mergePosition = current + 1;
                while (mergePosition < SIZE && grid[mergePosition][j] == 0) {
                    mergePosition++;
                }

                if (mergePosition < SIZE && grid[current][j] == grid[mergePosition][j] && lastMerged - current <= SIZE) {
                    grid[mergePosition][j] *= 2;
                    grid[current][j] = 0;
                    lastMerged = mergePosition;
                } else {
                    if (mergePosition - 1 != current) {
                        grid[mergePosition - 1][j] = grid[current][j];
                        grid[current][j] = 0;
                    }
                }
            }
        }
    }
    generateNewTile();
    glutPostRedisplay();
}

void renderGridAndTiles() {
    int temp = 0;
    int valueArray[] = {0};
    int gridSize = 4;
    int tileSize = 100;
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            int value = grid[i][j];
            float x = j * tileSize;
            float y = i * tileSize;
            if (value == 0) {
                glColor3f(0.7, 0.7, 0.7);
            } else if (value == 2) {
                glColor3f(1.0, 0.0, 0.0);
            } else if (value == 4) {
                glColor3f(0.0, 1.0, 0.0);
            } else if (value == 8) {
                glColor3f(0.0, 0.0, 1.0);
            } else if (value == 16) {
                glColor3f(0.0, 1.0, 1.0);
            } else if (value == 32) {
                glColor3f(0.0, 0.0, 1.0);
            } else if (value == 64) {
                glColor3f(1.0, 0.0, 1.0);
            } else if (value == 128) {
                glColor3f(1.0, 1.0, 1.0);
            } else if (value == 256) {
                glColor3f(0.0, 2.0, 1.0);
            } else if (value == 512) {
                glColor3f(2.0, 0.0, 1.0);
            } else if (value == 1024) {
                glColor3f(2.0, 2.0, 1.0);
            } else if (value == 2048) {
                glColor3f(0.0, 2.0, 2.0);
            }
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + tileSize, y);
            glVertex2f(x + tileSize, y + tileSize);
            glVertex2f(x, y + tileSize);
            glEnd();
            if (value != 0) {
                glColor3f(0.0, 0.0, 0.0);
                glRasterPos2f(x + 0.5 * tileSize, y + 0.5 * tileSize);
                while(value != 0)
                {
                    valueArray[temp] = value%10;
                    temp++;
                    value/=10;
                }
                for(int i=temp-1;i>=0;i--)
                {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, valueArray[i] + '0');
                }
                temp = 0;
            }
        }
        glColor3f(0.0, 0.0, 0.0);
        glLineWidth(3.0);
        glBegin(GL_LINES);
        for (int i = 0; i <= gridSize; ++i) {
            glVertex2f(0, i * tileSize);
            glVertex2f(gridSize * tileSize, i * tileSize);
            glVertex2f(i * tileSize, 0);
            glVertex2f(i * tileSize, gridSize * tileSize);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderGridAndTiles();
    glutSwapBuffers();
}

void handleKeyPress(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            moveTilesLeft();
            break;
        case GLUT_KEY_RIGHT:
            moveTilesRight();
            break;
        case GLUT_KEY_UP:
            moveTilesUp();
            break;
        case GLUT_KEY_DOWN:
            moveTilesDown();
            break;
    }
}

int main(int argc, char** argv) {
    srand(time(NULL));
    initializeGrid();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("2048");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 400, 400, 0);
    glutDisplayFunc(display);
    glutSpecialFunc(handleKeyPress);
    glutMainLoop();
    return 0;
}
