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
#include <string.h>
#include <GL/freeglut.h>

#define SIZE 4 // rozmiar siatki
#define SPEED 25*(SIZE/2) // szybkosc blokow

int grid[SIZE][SIZE]; // siatka

int emptySpaces,xd,yd; // puste miejsca (tiles z wartoscia '0')
int tileSize = 800/SIZE;
unsigned int score = 0;
unsigned int highscoreInt = 0;

FILE *file;

bool isGameStarted;
bool isInMenu;
bool m;

unsigned int resolutionChoice;

void mouseGame(int button, int state, int x, int y);
void mouseSettings(int button, int state, int x, int y);
void mouse(int button, int state, int x, int y);
void timer();
void playBackgroundMusic();
void scoreHandler(int mergedValue);

struct tilePos {
    int iDest;
    int jDest;
    int iCurrentCoords;
    int jCurrentCoords;
    
    int destValue;
    int sourceValue;
}animationPos[SIZE][SIZE];

struct Options {
    int resolutionWidth;
    int resolutionHeight;
    bool fullscreen;
    int size;
    bool infiniteMode;
    bool merge;
}Settings;

struct resolution {
    int width;
    int height;
}resolutions[7];


GLuint texture;

GLuint LoadTexture(const char* filename) {
    GLuint texture;
    int width, height;
    unsigned char* data;

    FILE* file = fopen(filename, "rb");
    if (file == NULL) return 0;

    fseek(file, 18, SEEK_SET);
    fread(&width, sizeof(int), 1, file);
    fread(&height, sizeof(int), 1, file);
    fseek(file, 54, SEEK_SET);

    data = (unsigned char*)malloc(width * height * 3);
    fread(data, width * height * 3, 1, file);
    fclose(file);

    for (int y=0;y<height/2;y++) {
        for (int x=0;x<width*3;x++) {
            unsigned char temp=data[y*width*3+x];
            data[y*width*3+x]=data[(height-1-y)*width*3+x];
            data[(height-1-y)*width*3+x]=temp;
        }
    }

    for (int i = 0; i < width * height; i++) {
        int index = i * 3;
        unsigned char B, R;
        B = data[index];
        R = data[index + 2];
        data[index] = R;
        data[index + 2] = B;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    free(data);

    return texture;
}

void resetanimationPos() {
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            animationPos[i][j].jDest=-1;
            animationPos[i][j].iDest=-1;
            animationPos[i][j].destValue=0;
            animationPos[i][j].sourceValue=0;
        }
    }
}

void showValue() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int current = grid[i][j];
            printf("[%d][%d] = %d\n", i, j, current);
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

void drawTile(int x, int y, int value, int valueArray[])
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
                    glColor3f(0.871,1.,1.);
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
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.0, 1.0, 1.0);
                    break;
                case 32:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0, 1.0, 1.0);
                    break;
                case 64:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
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

struct tile {
    int iCoord;
    int jCoord;
};

void generateNewTile() {
    int value = rand() % 3 == 0 ? 4 : 2;
    struct tile tempTab[SIZE*SIZE];
    int counter=0;
    for(int i=0;i<SIZE;i++) {
        for(int j=0;j<SIZE;j++) {
            if(grid[i][j]==0) {
                tempTab[counter].iCoord=i;
                tempTab[counter].jCoord=j;
                counter++;
            }
        }
    }
    if(counter==0)
        return;
    int randomIndex = rand()%counter;    

    grid[ tempTab[randomIndex].iCoord ][ tempTab[randomIndex].jCoord ] = value;
    // if (emptySpaces > 0) {
    //     int randomIndex = rand() % emptySpaces;
    //     int count = 0;
    //     for (int i = 0; i < SIZE; i++) {
    //         for (int j = 0; j < SIZE; j++) {
    //             if (grid[i][j] == 0) {
    //                 if (count == randomIndex) {
    //                     grid[i][j] = value;
    //                     return;
    //                 }
    //                 count++;
    //             }
    //         }
    //     }
    // }
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
                grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                yd=0;
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
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
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].jDest=mergePosition*tileSize;
                        animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[i][k].iDest=animationPos[i][k].iCurrentCoords=i*tileSize;
                    animationPos[i][k].jDest=mergePosition*tileSize;
                    animationPos[i][k].jCurrentCoords=k*tileSize;
                    animationPos[i][k].sourceValue=grid[i][k];
                    animationPos[i][k].destValue=grid[i][k]*2;
                    scoreHandler(grid[i][k]);
                    grid[i][k]=0;
                    mergePosition++;
                    break;
                }
                if((grid[i][j]!=grid[i][k] && grid[i][k]) || (k==SIZE-1 && !grid[i][k])) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].jDest=mergePosition*tileSize;
                        animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition++;
                    break;
                }
            }
            if(j==SIZE-1 && grid[i][j]) {
                if(mergePosition!=j) {
                    animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                    animationPos[i][j].jDest=mergePosition*tileSize;
                    animationPos[i][j].jCurrentCoords=j*tileSize;
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
                grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                yd=0;
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
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
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].jDest=mergePosition*tileSize;
                        animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[i][k].iDest=animationPos[i][k].iCurrentCoords=i*tileSize;
                    animationPos[i][k].jDest=mergePosition*tileSize;
                    animationPos[i][k].jCurrentCoords=k*tileSize;
                    animationPos[i][k].sourceValue=grid[i][k];
                    animationPos[i][k].destValue=grid[i][k]*2;
                    scoreHandler(grid[i][k]);
                    grid[i][k]=0;
                    mergePosition--;
                    break;
                }
                if((grid[i][j]!=grid[i][k] && grid[i][k]) || (k==0 && !grid[i][k])) {
                    if(mergePosition!=j) {
                        animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].jDest=mergePosition*tileSize;
                        animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition--;
                    break;
                }
            }
            if(j==0 && grid[i][j]) {
                if(mergePosition!=j) {
                    animationPos[i][j].iDest=animationPos[i][j].iCurrentCoords=i*tileSize;
                    animationPos[i][j].jDest=mergePosition*tileSize;
                    animationPos[i][j].jCurrentCoords=j*tileSize;
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
                grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                xd=0;
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
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
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].iDest=mergePosition*tileSize;
                        animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[k][j].jDest=animationPos[k][j].jCurrentCoords=j*tileSize;
                    animationPos[k][j].iDest=mergePosition*tileSize;
                    animationPos[k][j].iCurrentCoords=k*tileSize;
                    animationPos[k][j].sourceValue=grid[k][j];
                    animationPos[k][j].destValue=grid[k][j]*2;
                    scoreHandler(grid[k][j]);
                    grid[k][j]=0;
                    mergePosition++;
                    break;
                }
                if((grid[i][j]!=grid[k][j] && grid[k][j]) || (k==SIZE-1 && !grid[k][j])) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].iDest=mergePosition*tileSize;
                        animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition++;
                    break;
                }
            }
            if(i==SIZE-1 && grid[i][j]) {
                if(mergePosition!=i) {
                    animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                    animationPos[i][j].iDest=mergePosition*tileSize;
                    animationPos[i][j].iCurrentCoords=i*tileSize;
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
                grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
                animationPos[i][j].iDest=-1;
                animationPos[i][j].jDest=-1;
                xd=0;
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
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
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].iDest=mergePosition*tileSize;
                        animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    animationPos[k][j].jDest=animationPos[k][j].jCurrentCoords=j*tileSize;
                    animationPos[k][j].iDest=mergePosition*tileSize;
                    animationPos[k][j].iCurrentCoords=k*tileSize;
                    animationPos[k][j].sourceValue=grid[k][j];
                    animationPos[k][j].destValue=grid[k][j]*2;
                    scoreHandler(grid[k][j]);
                    grid[k][j]=0;
                    mergePosition--;
                    break;
                }
                if((grid[i][j]!=grid[k][j] && grid[k][j]) || (k==0 && !grid[k][j])) {
                    if(mergePosition!=i) {
                        animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                        animationPos[i][j].iDest=mergePosition*tileSize;
                        animationPos[i][j].iCurrentCoords=i*tileSize;
                        animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                        grid[i][j]=0;
                    }
                    mergePosition--;
                    break;
                }
            }
            if(i==0 && grid[i][j]) {
                if(mergePosition!=i) {
                    animationPos[i][j].jDest=animationPos[i][j].jCurrentCoords=j*tileSize;
                    animationPos[i][j].iDest=mergePosition*tileSize;
                    animationPos[i][j].iCurrentCoords=i*tileSize;
                    animationPos[i][j].sourceValue=animationPos[i][j].destValue=grid[i][j];
                    grid[i][j]=0;
                }
            }
        }
    }
    yd=SPEED;
    glutPostRedisplay();
}

void drawBitmapInt(unsigned int text, float offsetWidth, float offsetHeight) {
    int temp = 0;
    int valueArray[10];
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,Settings.resolutionWidth, Settings.resolutionHeight, 0);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(Settings.resolutionWidth*offsetWidth,Settings.resolutionHeight*offsetHeight);//(1135, 310);
    if (text != 0) {
        while(text != 0) {
            valueArray[temp] = text%10;
            temp++;
            text/=10;
        }
        for(int i=temp-1;i>=0;i--) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, valueArray[i] + '0');
        }
        temp = 0;
    }
    else glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0');
    glPopMatrix();
}

void drawBitmapString(char *text, float offsetWidth, float offsetHeight) {
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,Settings.resolutionWidth, Settings.resolutionHeight, 0);
    glScaled((Settings.resolutionHeight/3086.0),-(Settings.resolutionHeight/3086.0),1.0);
    glColor3f(1.0, 1.0, 1.0);
    //glRasterPos2f(Settings.resolutionWidth*offsetWidth,Settings.resolutionHeight*offsetHeight);
    glTranslated(Settings.resolutionWidth*offsetWidth,-Settings.resolutionHeight*offsetHeight,0);
    for(int i=0;i<strlen(text);i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
        //glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    glPopMatrix();
}

bool isBackgroundLoaded = false;
void renderGridAndTiles() {
    // int temp = 0;
    //static bool isBackgroundLoaded = false;
    int valueArray[4]; // tablica przechowywujaca wartosc tilesa do wypisania na nim
    //int gridSize = SIZE; // rozmiar siatki (4x4)
    //int tileSize = tileSize; // rozmiar tile (tileSizextileSize px)
    int counter = 0;
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
    if(!isBackgroundLoaded) {
        texture = LoadTexture("textures/inGame.bmp");
        isBackgroundLoaded=true;
    }
    glColor3f(1, 1, 1);
    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();
    gluOrtho2D(-140,1780,940,-140);
    glColor3f(0, 0, 0); // kolor linii
    glLineWidth(3.0); // grubosc linii
    // zaczecie rysowania linii
    glBegin(GL_LINES);
    for (int i = 0; i <= SIZE; i++) {
        glVertex2f(0, i * tileSize);
        glVertex2f(SIZE * tileSize, i * tileSize);
        glVertex2f(i * tileSize, 0);
        glVertex2f(i * tileSize, SIZE * tileSize);
    }
    glEnd();
    glPopMatrix();

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int value = grid[i][j];
            bool isEmpty = true;
            float x = j * tileSize;
            float y = i * tileSize;
            if(animationPos[i][j].iDest!=-1) {
                drawTile(x,y,animationPos[i][j].sourceValue,valueArray);
                if(animationPos[i][j].jCurrentCoords==animationPos[i][j].jDest && 
                    animationPos[i][j].iCurrentCoords==animationPos[i][j].iDest) {
                    //grid[animationPos[i][j].iSource][animationPos[i][j].jSource]=0;
                    grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
                    animationPos[i][j].iDest=-1;
                    animationPos[i][j].jDest=-1;
                    counter++;
                }
            } 
            else {
                glPushMatrix();
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
                drawTile(x,y,value,valueArray);
                glPopMatrix();
            }
            for(int k=0;k<SIZE;k++) {
                for(int l=0;l<SIZE;l++) {
                    if(animationPos[k][l].iDest!=-1)
                        isEmpty=false;
                }
            }
            // if(isButtonPressed) {
            //     for(int i=0;i<SIZE;i++) {
            //         for(int j=0;j<SIZE;j++) {
            //             if(animationPos[i][j].iDest!=-1) {
            //                 grid[ (animationPos[i][j].iDest)/tileSize ][ (animationPos[i][j].jDest)/tileSize ]=animationPos[i][j].destValue;
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
                    //updateEmptySpaces();
                    //printf("%d ",emptySpaces);
                    //showValue();
                    counter=0;
                }
                glLoadIdentity();
                gluOrtho2D(-140,1780,940,-140);
                xd=0;
                yd=0;
            }
            glPushMatrix();
            glLoadIdentity();
            gluOrtho2D(-140,1780,940,-140);
            glColor3f(0, 0, 0); // kolor linii
            glLineWidth(5.0); // grubosc linii
            // zaczecie rysowania linii
            glBegin(GL_LINES);
                glVertex2f(0, 0);
                glVertex2f(0, SIZE*tileSize);
                glVertex2f(0, SIZE*tileSize);
                glVertex2f(SIZE*tileSize, SIZE*tileSize);
                glVertex2f(SIZE*tileSize, SIZE*tileSize);
                glVertex2f(SIZE*tileSize, 0);
                glVertex2f(SIZE*tileSize, 0);
                glVertex2f(0, 0);
            glEnd();
            glPopMatrix();

            drawBitmapInt(score,0.76,0.407);
            drawBitmapInt(highscoreInt,0.76,0.265);
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderGridAndTiles();
    updateEmptySpaces();

    if(checkWin()) {
        printf("wygrales!");
        exit(0);
    }
    if(checkGameOver()) {
        printf("przegrales!");
        exit(0);
    }
    glutSwapBuffers();
}

void scoreHandler(int mergedValue) {
    // static int score = 0;
    if(isGameStarted) {
        score+=mergedValue*2;
        printf("Wynik: %u\n",score);
        if(score>highscoreInt) {
            highscoreInt=score;
            printf("Wiekszy: %u",highscoreInt);
            fseek(file,0,SEEK_SET);
            fprintf(file,"%u",highscoreInt);
        }
    }
    else score = 0; 
}

void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    texture = LoadTexture("textures/menu.bmp");
    //glBindTexture(GL_TEXTURE_2D, texture);
   
    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();

    texture = LoadTexture("textures/quitButton.bmp");

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    // Exit 
   // glBindTexture(GL_TEXTURE_2D, texture);
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)));
    // glTexCoord2d(0.0, 0.0); glVertex2d(150, 200);
    // glTexCoord2d(1.0, 0.0); glVertex2d(250, 200);
    // glTexCoord2d(1.0, 1.0); glVertex2d(250, 230);
    // glTexCoord2d(0.0, 1.0); glVertex2d(150, 230);
    glEnd();

    texture = LoadTexture("textures/settingsButton.bmp");

    // Credits/Settings
    glBegin(GL_QUADS);
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glEnd();

    texture = LoadTexture("textures/startButton.bmp");

    // Start
    glBegin(GL_QUADS);
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glEnd();

    glutPostRedisplay();
    glutSwapBuffers();
}

void display3() {
    glClear(GL_COLOR_BUFFER_BIT);
    static char temp[10], temp2[10];
    texture = LoadTexture("textures/settings.bmp");
    //glBindTexture(GL_TEXTURE_2D, texture);
    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();

    texture = LoadTexture("textures/backButton.bmp");

    // Credits/Settings
    glBegin(GL_QUADS);
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexCoord2d(0.0, 0.0); glVertex2d(Settings.resolutionWidth*0.833, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth*0.931, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth*0.931, Settings.resolutionHeight*0.846);
    glTexCoord2d(0.0, 1.0); glVertex2d(Settings.resolutionWidth*0.833, Settings.resolutionHeight*0.846);
    glEnd();

    glBegin(GL_QUADS);
    //glBindTexture(GL_TEXTURE_2D, texture);
    glTexCoord2d(0.0, 0.0); glVertex2d(Settings.resolutionWidth*0.069, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth*0.167, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth*0.167, Settings.resolutionHeight*0.846);
    glTexCoord2d(0.0, 1.0); glVertex2d(Settings.resolutionWidth*0.069, Settings.resolutionHeight*0.846);
    glEnd();

    // Display resolution option
    itoa(resolutions[resolutionChoice].width,temp,10);
    itoa(resolutions[resolutionChoice].height,temp2,10);
    strcat(temp,"x");
    strcat(temp,temp2);
    if(!resolutionChoice) strcat(temp," (native)");
    
    drawBitmapString(temp,633.6/Settings.resolutionWidth,583.2/Settings.resolutionHeight);

    glutSwapBuffers();
}

//int id=1;

void handleKeyPress(int key, int x, int y) {
    if(isGameStarted) {
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
}

// debilo-odporna funkcja
void resetGrid() {
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++) {
            grid[i][j] = 0;
        }
    }
}

void mouseMenu(int button, int state, int x, int y) {
    // static int d = 0;
    //if(!isGameStarted && isInMenu) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // start
        if (x >= ((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10 && x <= ((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10 
            && y >= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)) 
                && y <= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40))) {
            isGameStarted = true;
            m=false;
            srand(time(NULL));
            //glDisable(GL_TEXTURE_2D);
            resetanimationPos();
            initializeGrid();
            glMatrixMode(GL_MODELVIEW);
            glutDisplayFunc(display);
            glutReportErrors();
            glutSpecialFunc(handleKeyPress);
            glutMouseFunc(mouseGame);
            if(!m){
                glutTimerFunc(0,timer,0);
                m=true;
            }
            glutPostRedisplay();
            //playBackgroundMusic("music//DS.wav");
            printf("Start");
        }
        // credits
        else if (x >= ((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10 && x <= ((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10 
        && y >= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40))
        && y <= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40))) {
            glutDisplayFunc(display3);
            glutMouseFunc(mouseSettings);
            isInMenu = false;
            glMatrixMode(GL_MODELVIEW);
            printf("settings");
            glutPostRedisplay();
        }
        // quit
        else if (x >= ((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10 && x <= ((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10 
            && y >= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10))
                && y <= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20))) { 
            exit(0);
        }
    }
    //}
}

void mouseSettings(int button, int state, int x, int y) {
    //if(!isGameStarted && !isInMenu) {
    static int choiceCheck = 0;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // back do menu
        if(x>=Settings.resolutionWidth*0.833 && x<=Settings.resolutionWidth*0.931 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            resolutionChoice=choiceCheck;
            glutDisplayFunc(display2);
            glutMouseFunc(mouseMenu);
            isInMenu = true;
            printf("settings");
        }

        // Apply
        if(x>=Settings.resolutionWidth*0.069 && x<=Settings.resolutionWidth*0.167 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            if(resolutionChoice!=choiceCheck) {
                Settings.resolutionWidth=resolutions[resolutionChoice].width;
                Settings.resolutionHeight=resolutions[resolutionChoice].height;
                glutReshapeWindow(Settings.resolutionWidth, Settings.resolutionHeight);
                choiceCheck=resolutionChoice;
                // printf("\nSettings: %dx%d, resolutions: %dx%d, choice: %d, choiceCheck: %d", Settings.resolutionWidth, Settings.resolutionHeight, resolutions[resolutionChoice].width, resolutions[resolutionChoice].height, resolutionChoice, choiceCheck);
            }
            isInMenu = true;
            glLoadIdentity();
            gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
            glutDisplayFunc(display2);
            glutMouseFunc(mouseMenu);
        }

        if(x>=Settings.resolutionWidth*0.0 && x<=Settings.resolutionWidth*0.2 
        && y>=Settings.resolutionHeight*0.0 && y<=Settings.resolutionHeight*0.2
        && resolutionChoice>=1) {
            resolutionChoice--;
        }

        if(x>=Settings.resolutionWidth*0.8 && x<=Settings.resolutionWidth*1.0 
        && y>=Settings.resolutionHeight*0.0 && y<=Settings.resolutionHeight*0.2
        && resolutionChoice<=5) {
            resolutionChoice++;
        }
    }
}

void mouseGame(int button, int state, int x, int y) {
    //if(isGameStarted) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(x>=Settings.resolutionWidth*0.833 && x<=Settings.resolutionWidth*0.931 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            glLoadIdentity();
            gluOrtho2D(0, Settings.resolutionWidth, Settings.resolutionHeight, 0);
            glEnable(GL_TEXTURE_2D);
            glutDisplayFunc(display2);
            glutSpecialFunc(NULL);
            glutMouseFunc(mouseMenu);
            isInMenu = true;
            isGameStarted = false;
            isBackgroundLoaded = false;
            score = 0;
        }
    }
    //} //printf("x: %d, y: %d\n", x, y);
}

void timer() {
    if(isGameStarted) {
        glutPostRedisplay();
        glutTimerFunc(1000/60,timer,0);

        if(xd!=0 || yd!=0) {
            for(int i=0;i<SIZE;i++) {
                for(int j=0;j<SIZE;j++) {
                    if(animationPos[i][j].iDest!=-1) {
                        animationPos[i][j].iCurrentCoords+=yd;
                        animationPos[i][j].jCurrentCoords+=xd;
                    }
                }
            }
            glTranslated(xd,yd,0);
        }
    }
}

void playBackgroundMusic(const char* filePath) {
    PlaySound(filePath, NULL, SND_ASYNC | SND_LOOP);
}

int main(int argc, char** argv) {
    //playBackgroundMusic("music//menu.wav");
    isGameStarted = false;
    isInMenu = true;
    
    file = fopen("scoreboard.mp3","r+");
    fscanf(file,"%u",&highscoreInt);

    printf("highscore: %u\t",highscoreInt);

    glutInit(&argc, argv);
    Settings.resolutionWidth = glutGet(GLUT_SCREEN_WIDTH);
    Settings.resolutionHeight = glutGet(GLUT_SCREEN_HEIGHT);

    resolutions[0].width = glutGet(GLUT_SCREEN_WIDTH);
    resolutions[0].height = glutGet(GLUT_SCREEN_HEIGHT);
    resolutions[1].width = 960;
    resolutions[1].height = 540;
    resolutions[2].width = 1024;
    resolutions[2].height = 576;
    resolutions[3].width = 1280;
    resolutions[3].height = 720;
    resolutions[4].width = 1366;
    resolutions[4].height = 768;
    resolutions[5].width = 1600;
    resolutions[5].height = 900;
    resolutions[6].width = 1920;
    resolutions[6].height = 1080;

    printf("%d %d",Settings.resolutionWidth,Settings.resolutionHeight);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Settings.resolutionWidth,Settings.resolutionHeight);
    glutCreateWindow("2048");
    glutFullScreen();
    gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glEnable( GL_TEXTURE_2D );
    glutDisplayFunc(display2);
    //glutIdleFunc(display2);
    glutMouseFunc(mouseMenu);
    // srand(time(NULL)); // generowanie seedu
    // resetanimationPos();
    // initializeGrid(); // inicjalizacja siatki gry
    // glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    // glutInitWindowSize(400, 400);
    // glutCreateWindow("2048");
    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluOrtho2D(-148,1772,938,-148); // ustawienie obszaru ortogonalnego
    // glutDisplayFunc(display);
    // glutReportErrors();
    // glutSpecialFunc(handleKeyPress);
    // glutTimerFunc(0,timer,0);
    // playBackgroundMusic();
    glutMainLoop();
    return 0;
}