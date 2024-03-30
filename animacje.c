/**
 * @file main.c
 * @brief Główny plik programu 2048.
 *
 * Ten plik zawiera funkcję `main` oraz podstawową logikę gry 2048.
 */

#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h> 
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>
#include <stdbool.h>

#define SIZE 4 // rozmiar siatki

#define SPEED 25 // szybkosc blokow

int grid[SIZE][SIZE]; // siatka

int emptySpaces,xd,yd; // puste miejsca (tiles z wartoscia '0')

struct tilePos {
    int iDest;
    int jDest;
    int iCurrentCoords;
    int jCurrentCoords;
    
    int destValue;
    int sourceValue;
}animationPos[SIZE][SIZE];

void resetanimationPos() {
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            animationPos[i][j].jDest=-1;
            animationPos[i][j].iDest=-1;
            animationPos[i][j].destValue=0;
            animationPos[i][j].sourceValue=0;
        }
    }
}

void updateEmptySpaces() {
    emptySpaces = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 0) {
                emptySpaces++;
            }
        }
    }
}

void drawTile(int x, int y, int tileSize, int value, int valueArray[])
{
    int temp=0;
    switch(value) {
                case 0:
                    //glColor3f(0.7, 0.7, 0.7);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable( GL_BLEND );
                    glColor4f(0.0, 0.0, 0.0, 0.0);
                    break;
                case 2:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0, 0.0, 0.0);
                    break;
                case 4:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 1.0, 0.0);
                    break;
                case 8:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 0.0, 1.0);
                    break;
                case 16:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 1.0, 1.0);
                    break;
                case 32:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0, 1.0, 1.0);
                    break;
                case 64:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.3, 0.6, 1.0);
                    break;
                case 128:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0, 0.0, 1.0);
                    break;
                case 256:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0, 0.4, 0.5);
                    break;
                case 512:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 1.0, 0.5);
                    break;
                case 1024:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.5, 0.4, 0.0);
                    break;
                case 2048:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 0.5, 0.5);
                    break;
            }
            // zaczecie rysowania tilesow
                glBegin(GL_QUADS);
                glVertex2f(x, y);
                glVertex2f(x + tileSize, y);
                glVertex2f(x + tileSize, y + tileSize);
                glVertex2f(x, y + tileSize);
                glEnd();
            if (value != 0) {
                while(value != 0) {
                    valueArray[temp] = value%10;
                    temp++;
                    value/=10;
                }
                for(int i=temp-1;i>=0;i--) {
                    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, valueArray[i] + '0');
                }
                temp = 0;
            }
    //glLoadIdentity();
    // glBegin(GL_QUADS);
    // glVertex2f(x, y);
    // glVertex2f(x + tileSize, y);
    // glVertex2f(x + tileSize, y + tileSize);
    // glVertex2f(x, y + tileSize);
    // glEnd();
}

// void moveTile(int *x, int *y, int xd, int yd, int tileSize)
// {
//     *x+=xd;
//     *y+=yd;
//     drawTile(*x,*y,tileSize);
// }

void generateNewTile() {
    int value = rand() % 3 == 0 ? 4 : 2;
    if (emptySpaces > 0) {
        int randomIndex = rand() % emptySpaces;
        int count = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = 0;
        }
    }
    updateEmptySpaces();
    generateNewTile();
    generateNewTile();
}

void moveTilesLeft() {
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(animationPos[i][j].iDest!=-1) {
                animationPos[i][j].iCurrentCoords=animationPos[i][j].iDest;
                animationPos[i][j].jCurrentCoords=animationPos[i][j].jDest;
                grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                yd=0;
                glLoadIdentity();
                gluOrtho2D(0,400,400,0);
                generateNewTile();
                // counter=1;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        // int lastMerged = -1;
        int mergePosition = 0;
        // od lewej do prawej
        for (int j = 0; j < SIZE; j++) {
            if(!grid[i][j])
                continue;
            for(int k=j+1;k<SIZE;k++) {
                if(grid[i][j]==grid[i][k]) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].jDest=mergePosition*100;
                        animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[i][k].iDest=animationPos[i][k].iCurrentCoords=i*100;
                    animationPos[i][k].jDest=mergePosition*100;
                    animationPos[i][k].jCurrentCoords=k*100;
                    animationPos[i][k].sourceValue=grid[i][k];
                    animationPos[i][k].destValue=grid[i][k]*2;
                    grid[i][k]=0;
                    mergePosition++;
                    break;
                }
                if((grid[i][j]!=grid[i][k] && grid[i][k]) || (k==SIZE-1 && !grid[i][k])) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].jDest=mergePosition*100;
                        animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition++;
                    break;
                }
            }
            if(j==SIZE-1 && grid[i][j]) {
                if(mergePosition!=j) {
                    animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                    animationPos[i][j].jDest=mergePosition*100;
                    animationPos[i][j].jCurrentCoords=j*100;
                    animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                    grid[i][j]=0;
                }
            }
        }
    }
    xd=-SPEED;
    glutPostRedisplay();
}

void moveTilesRight() {
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(animationPos[i][j].iDest!=-1) {
                animationPos[i][j].iCurrentCoords=animationPos[i][j].iDest;
                animationPos[i][j].jCurrentCoords=animationPos[i][j].jDest;
                grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                yd=0;
                glLoadIdentity();
                gluOrtho2D(0,400,400,0);
                generateNewTile();
                // counter=1;
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
        int mergePosition = SIZE-1;
        // od lewej do prawej
        for (int j = SIZE-1; j >=0; j--) {
            if(!grid[i][j])
                continue;
            for(int k=j-1;k>=0;k--) {
                if(grid[i][j]==grid[i][k]) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].jDest=mergePosition*100;
                        animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[i][k].iDest=animationPos[i][k].iCurrentCoords=i*100;
                    animationPos[i][k].jDest=mergePosition*100;
                    animationPos[i][k].jCurrentCoords=k*100;
                    animationPos[i][k].sourceValue=grid[i][k];
                    animationPos[i][k].destValue=grid[i][k]*2;
                    grid[i][k]=0;
                    mergePosition--;
                    break;
                }
                if((grid[i][j]!=grid[i][k] && grid[i][k]) || (k==0 && !grid[i][k])) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].jDest=mergePosition*100;
                        animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition--;
                    break;
                }
            }
            if(j==0 && grid[i][j]) {
                if(mergePosition!=j) {
                    animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*100;
                    animationPos[i][j].jDest=mergePosition*100;
                    animationPos[i][j].jCurrentCoords=j*100;
                    animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                    grid[i][j]=0;
                }
            }
        }
    }
    xd=SPEED;
    glutPostRedisplay();
}

void moveTilesUp() {
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(animationPos[i][j].iDest!=-1) {
                animationPos[i][j].iCurrentCoords=animationPos[i][j].iDest;
                animationPos[i][j].jCurrentCoords=animationPos[i][j].jDest;
                grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                xd=0;
                glLoadIdentity();
                gluOrtho2D(0,400,400,0);
                generateNewTile();
                // counter=1;
            }
        }
    }
    for (int j = 0; j < SIZE; j++) {
        int mergePosition = 0;
        // od lewej do prawej
        for (int i = 0; i < SIZE; i++) {
            if(!grid[i][j])
                continue;
            for(int k=i+1;k<SIZE;k++) {
                if(grid[i][j]==grid[k][j]) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].iDest=mergePosition*100;
                        animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[k][j].jDest=animationPos[k][j].jCurrentCoords=j*100;
                    animationPos[k][j].iDest=mergePosition*100;
                    animationPos[k][j].iCurrentCoords=k*100;
                    animationPos[k][j].sourceValue=grid[k][j];
                    animationPos[k][j].destValue=grid[k][j]*2;
                    grid[k][j]=0;
                    mergePosition++;
                    break;
                }
                if((grid[i][j]!=grid[k][j] && grid[k][j]) || (k==SIZE-1 && !grid[k][j])) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].iDest=mergePosition*100;
                        animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition++;
                    break;
                }
            }
            if(i==SIZE-1 && grid[i][j]) {
                if(mergePosition!=i) {
                    animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                    animationPos[i][j].iDest=mergePosition*100;
                    animationPos[i][j].iCurrentCoords=i*100;
                    animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                    grid[i][j]=0;
                }
            }
        }
    }
    yd=-SPEED;
    glutPostRedisplay();
}

void moveTilesDown() {
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(animationPos[i][j].iDest!=-1) {
                animationPos[i][j].iCurrentCoords=animationPos[i][j].iDest;
                animationPos[i][j].jCurrentCoords=animationPos[i][j].jDest;
                grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                xd=0;
                glLoadIdentity();
                gluOrtho2D(0,400,400,0);
                generateNewTile();
                // counter=1;
            }
        }
    }
    for (int j = 0; j < SIZE; j++) {
        int mergePosition = SIZE-1;
        // od lewej do prawej
        for (int i = SIZE-1; i >= 0; i--) {
            if(!grid[i][j])
                continue;
            for(int k=i-1;k>=0;k--) {
                if(grid[i][j]==grid[k][j]) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].iDest=mergePosition*100;
                        animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[k][j].jDest=animationPos[k][j].jCurrentCoords=j*100;
                    animationPos[k][j].iDest=mergePosition*100;
                    animationPos[k][j].iCurrentCoords=k*100;
                    animationPos[k][j].sourceValue=grid[k][j];
                    animationPos[k][j].destValue=grid[k][j]*2;
                    grid[k][j]=0;
                    mergePosition--;
                    break;
                }
                if((grid[i][j]!=grid[k][j] && grid[k][j]) || (k==0 && !grid[k][j])) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                        animationPos[i][j].iDest=mergePosition*100;
                        animationPos[i][j].iCurrentCoords=i*100;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition--;
                    break;
                }
            }
            if(i==0 && grid[i][j]) {
                if(mergePosition!=i) {
                    animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*100;
                    animationPos[i][j].iDest=mergePosition*100;
                    animationPos[i][j].iCurrentCoords=i*100;
                    animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                    grid[i][j]=0;
                }
            }
        }
    }
    yd=SPEED;
    glutPostRedisplay();
}

void renderGridAndTiles() {
    // int temp = 0;
    int valueArray[4]; // tablica przechowywujaca wartosc tilesa do wypisania na nim
    int gridSize = SIZE; // rozmiar siatki (4x4)
    int tileSize = 100; // rozmiar tile (100x100 px)
    int counter = 0;
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,400,400,0);
    glColor3f(0.7, 0.7, 0.7); // kolor linii
    glLineWidth(3.0); // grubosc linii
    // zaczecie rysowania linii
    glBegin(GL_LINES);
    for (int i = 0; i <= gridSize; i++) {
        glVertex2f(0, i * tileSize);
        glVertex2f(gridSize * tileSize, i * tileSize);
        glVertex2f(i * tileSize, 0);
        glVertex2f(i * tileSize, gridSize * tileSize);
    }
    glEnd();
    glPopMatrix();

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            int value = grid[i][j];
            bool isEmpty = true;
            float x = j * tileSize;
            float y = i * tileSize;
            if(animationPos[i][j].iDest!=-1) {
                drawTile(x,y,tileSize,animationPos[i][j].sourceValue,valueArray);
                if(animationPos[i][j].jCurrentCoords==animationPos[i][j].jDest && animationPos[i][j].iCurrentCoords==animationPos[i][j].iDest) {
                    //grid[animationPos[i][j].iSource][animationPos[i][j].jSource]=0;
                    grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
                    animationPos[i][j].iDest=-1;
                    animationPos[i][j].jDest=-1;
                    counter++;
                }
            } 
            else {
                glPushMatrix();
                glLoadIdentity();
                gluOrtho2D(0, 400, 400, 0);
                drawTile(x,y,tileSize,value,valueArray);
                glPopMatrix();
            }
            for(int k=0;k<4;k++) {
                for(int l=0;l<4;l++) {
                    if(animationPos[k][l].iDest!=-1)
                        isEmpty=false;
                }
            }
            // if(isButtonPressed) {
            //     for(int i=0;i<SIZE;i++) {
            //         for(int j=0;j<SIZE;j++) {
            //             if(animationPos[i][j].iDest!=-1) {
            //                 grid[ (animationPos[i][j].iDest)/100 ][ (animationPos[i][j].jDest)/100 ]=animationPos[i][j].destValue;
            //                 animationPos[i][j].iDest=-1;
            //                 animationPos[i][j].jDest=-1;
            //                 counter=1;
            //             }
            //         }
            //     }
            // }
            if(isEmpty) {
                if(counter) {
                    generateNewTile();
                    counter=0;
                }
                glLoadIdentity();
                gluOrtho2D(0, 400, 400, 0);
                xd=0;
                yd=0;
            }
            // switch(value) {
            //     case 0:
            //         glColor3f(0.7, 0.7, 0.7);
            //         break;
            //     case 2:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(1.0, 0.0, 0.0);
            //         break;
            //     case 4:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.0, 1.0, 0.0);
            //         break;
            //     case 8:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.0, 0.0, 1.0);
            //         break;
            //     case 16:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.0, 1.0, 1.0);
            //         break;
            //     case 32:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(1.0, 1.0, 1.0);
            //         break;
            //     case 64:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.3, 0.6, 1.0);
            //         break;
            //     case 128:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(1.0, 0.0, 1.0);
            //         break;
            //     case 256:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(1.0, 0.4, 0.5);
            //         break;
            //     case 512:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.0, 1.0, 0.5);
            //         break;
            //     case 1024:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.5, 0.4, 0.0);
            //         break;
            //     case 2048:
            //         glColor3f(0.0, 0.0, 0.0);
            //         glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
            //         glColor3f(0.0, 0.5, 0.5);
            //         break;
            // }
            // // zaczecie rysowania tilesow
            // drawTile(x, y, tileSize);
            // if (value != 0) {
            //     while(value != 0) {
            //         valueArray[temp] = value%10;
            //         temp++;
            //         value/=10;
            //     }
            //     for(int i=temp-1;i>=0;i--) {
            //         glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, valueArray[i] + '0');
            //     }
            //     temp = 0;
            // }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderGridAndTiles();
    glutSwapBuffers();
}

int checkGameOver() {
    // sprawdzenie czy sa puste miejsca na planszy
    if (emptySpaces > 0)
        return 0; // gracz nie przegral, bo sa jeszcze puste miejsca

    // sprawdzenie czy sa mozliwe ruchy w lewo, prawo, gore lub dol
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int current = grid[i][j];
            // sprawdzenie ruchu w lewo
            if (j > 0 && grid[i][j - 1] == current)
                return 0;
            // sprawdzenie ruchu w prawo
            if (j < SIZE - 1 && grid[i][j + 1] == current)
                return 0;
            // sprawdzenie ruchu w górę
            if (i > 0 && grid[i - 1][j] == current)
                return 0;
            // sprawdzenie ruchu w dol
            if (i < SIZE - 1 && grid[i + 1][j] == current)
                return 0;
        }
    }
    // brak mozliwych ruchow, gracz przegral
    return 1;
}

int checkWin() {
    // sprawdzenie każdego kafelka na planszy
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (grid[i][j] == 2048) // jesli wartosc kafelka wynosi 2048, gracz wygrywa
                return 1;
        }
    }
    return 0; // Gracz nie wygrał
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

    if(checkGameOver())
    {
        printf("przegrales!");
        exit(0);
    }
    if(checkWin())
    {
        printf("wygrales!");
        exit(0);
    }
}

void timer() {
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer,0);

    if(xd!=0 || yd!=0) {
        for(int i=0;i<4;i++) {
            for(int j=0;j<4;j++) {
                if(animationPos[i][j].iDest!=-1) {
                    animationPos[i][j].iCurrentCoords+=yd;
                    animationPos[i][j].jCurrentCoords+=xd;
                }
            }
        }
        glTranslated(xd,yd,0);
    }
}

void playBackgroundMusic() {
    PlaySound(TEXT("music//AS.wav"), NULL, SND_ASYNC | SND_LOOP);
}

int main(int argc, char** argv) {
    srand(time(NULL)); // generowanie seedu
    resetanimationPos();
    initializeGrid(); // inicjalizacja siatki gry
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("2048");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 400, 400, 0); // ustawienie obszaru ortogonalnego
    glutDisplayFunc(display);
    glutReportErrors();
    glutSpecialFunc(handleKeyPress);
    glutTimerFunc(0,timer,0);
    playBackgroundMusic();
    glutMainLoop();
    return 0;
}
