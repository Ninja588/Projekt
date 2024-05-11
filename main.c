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

#include "headers/score.h"
#include "headers/globals.h"
#include "headers/animations.h"
#include "headers/text.h"
#include "headers/consts.h"

/**
 * @brief Sprawdza położenie myszki na ekranie w grze po starcie gry.
 * @param button Przycisk myszki.
 * @param state Stan przycisku myszki (wciśnięty czy nie itp).
 * @param x Pozycja x ekranu.
 * @param y Pozycja y ekranu. 
 * Funkcja sprawdza położenie myszki i jeśli jest na odpowiednich
 * koordynatach oraz gracz kliknał LPM, to wtedy wywołuje się odpowiednia akcja, czyli
 * powrót do menu gry.
 */
void mouseGame(int button, int state, int x, int y);

/**
 * @brief Sprawdza położenie myszki na ekranie w menu opcji.
 * @param button Przycisk myszki.
 * @param state Stan przycisku myszki (wciśnięty czy nie itp).
 * @param x Pozycja x ekranu.
 * @param y Pozycja y ekranu.
 * Funkcja sprawdza położenie myszki i jeśli jest na odpowiednich
 * koordynatach oraz gracz kliknał LPM, to wtedy wywołuje się odpowiednia akcja.
 * Na przykład zmiana rozdzielczości, czy włączenie trybu Infinite.
 */
void mouseSettings(int button, int state, int x, int y);

/**
 * @brief Sprawdza położenie myszki na ekranie w menu głównym.
 * @param button Przycisk myszki.
 * @param state Stan przycisku myszki (wciśnięty czy nie itp).
 * @param x Pozycja x ekranu.
 * @param y Pozycja y ekranu.
 * Funkcja sprawdza położenie myszki i jeśli jest na odpowiednich
 * koordynatach oraz gracz kliknał LPM, to wtedy wywołuje się odpowiednia akcja.
 * Na przykład wejście do gry, menu opcji lub wyjście z gry.
 */
void mouseMenu(int button, int state, int x, int y);

/**
 * @brief Służy do odtwarzania muzyki w tle.
 * @param filePath Ścieżka do pliku z muzyką.
 */
void playBackgroundMusic(const char* filePath) {
    PlaySound(filePath, NULL, SND_ASYNC | SND_LOOP);
}

/**
 * @brief Funkcja ładująca teksture.
 * @param filename Ścieżka do pliku z teksturą.
 * @return Zwracanie tekstury która się renderuje.
 */
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

/**
 * @brief Aktualizuje liczbę pustych miejsc na siatce.
 * 
 * Funkcja przelicza ilość pustych miejsc na siatce, czyli tile'ów z wartością '0',
 * i zapisuje tę wartość w zmiennej globalnej emptySpaces.
 */
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
/**
 * @brief Funkcja rysująca płytki na ekranie.
 * @param x Współrzędna x ekranu gdzie ma zacząć rysować.
 * @param y Współrzędna y ekranu gdzie ma zacząć rysować.
 * @param value Wartość płytki.
 * @param valueArray Tablica przechowywująca liczby do wypisania na płytkach.
 */
void drawTile(int x, int y, int value, int valueArray[])
{
    int temp=0;
switch(value) {
                default:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.0,0.0,0.0);
                    break;
                case 0:
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    glEnable( GL_BLEND );
                    glColor4f(0.0, 0.0, 0.0, 0.0);
                    break;
                case 2:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(1.,1.,1.);
                    break;
                case 4:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.725,0.929,0.98);
                    break;
                case 8:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.004,0.918,0.98);
                    break;
                case 16:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.176,0.58,0.89);
                    break;
                case 32:
                    glColor3f(1.0, 1.0, 1.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.067,0.204,0.98);
                    break;
                case 64:
                    glColor3f(1.0, 1.0, 1.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.086,0.,0.478);
                    break;
                case 128:
                    glColor3f(1.0, 1.0, 1.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.412,0.,0.878);
                    break;
                case 256:
                    glColor3f(1.0, 1.0, 1.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.475,0.224,0.482);
                    break;
                case 512:
                    glColor3f(1.0, 1.0, 1.0);
                    glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.475,0.004,0.663);
                    break;
                case 1024:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.929,0.,0.961);
                    break;
                case 2048:
                    glColor3f(0.0, 0.0, 0.0);
                    glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
                    glColor3f(0.922,0.,0.286);
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
}

/**
 * @brief Inicjalizuje siatkę i generuje 2 początkowe tilesy.
 * 
 * Funkcja przygotowuje początkową siatkę gry ustawiając wszystkie wartości na '0',
 * a następnie generuje dwa losowe tile'y na siatce.
 */
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

/// @brief Zmienna określająca czy tło w grze zostało załadowane.
bool isBackgroundLoaded = false;
/**
 * @brief Renderuje siatkę i tilesy.
 * Funkcja renderująca pole do gry, wczytująca tło
 * oraz pokazująca wynik i najlepszy wynik.
 */
void renderGridAndTiles() {
    int valueArray[4]; // tablica przechowywujaca wartosc tilesa do wypisania na nim
    int counter = 0;
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
    if(!isBackgroundLoaded) {
        GLuint texture;
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
            if(isEmpty) {
                if(counter) {
                    generateNewTile();
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
        }
    }
}
/**
 * @brief Sprawdza, czy gracz przegrał.
 * 
 * Funkcja sprawdza, czy gracz przegrał, czyli czy nie ma możliwych ruchów na planszy.
 * @return 1, jeśli gracz przegrał, w przeciwnym razie 0.
 */
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
/**
 * @brief Sprawdza, czy gracz wygrał.
 * 
 * Funkcja sprawdza, czy gracz osiągnął wartość 2048 na jednym z tile'ów.
 * @return 1, jeśli gracz wygrał, w przeciwnym razie 0.
 */
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

/**
 * @brief Funkcja wyświetlająca ekran wygranej.
 */
void displayWin() {
    glEnable(GL_TEXTURE_2D);
    GLuint texture;
    texture = LoadTexture("textures/winScreen.bmp");
    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawBitmapInt(score,0.76,0.407);
    drawBitmapInt(highscoreInt,0.76,0.265);
    glutSwapBuffers();
}

/**
 * @brief Funkcja wyświetlająca ekran przegranej.
 */
void displayLose() {
    glEnable(GL_TEXTURE_2D);
    GLuint texture;
    texture = LoadTexture("textures/loseScreen.bmp");
    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    drawBitmapInt(score,0.76,0.407);
    drawBitmapInt(highscoreInt,0.76,0.265);
    glutSwapBuffers();
}

/**
 * @brief Funkcja wyświetlająca gre.
 * 
 * Funkcja wyświetla zawartość okna gry używając poprzednich funkcji.
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    renderGridAndTiles();
    updateEmptySpaces();

    if(checkWin() && !Settings.infiniteMode) {
        glutSpecialFunc(NULL);
        glLoadIdentity();
        gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
        isGameStarted=false;
        glutDisplayFunc(displayWin);
        glutPostRedisplay();
    }
    if(checkGameOver()) {
        glutSpecialFunc(NULL);
        glLoadIdentity();
        gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
        isGameStarted=false;
        glutDisplayFunc(displayLose);
        glutPostRedisplay();
    }
    glutSwapBuffers();
}

/**
 * @brief Funkcja wyświetlająca menu gry.
 */
void display2() {
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint texture;
    texture = LoadTexture("textures/menu.bmp");
   
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
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)));
    glEnd();

    texture = LoadTexture("textures/settingsButton.bmp");

    // Credits/Settings
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40)));
    glEnd();

    texture = LoadTexture("textures/startButton.bmp");

    // Start
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 0.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(1.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glTexCoord2d(0.0, 1.0); glVertex2d(((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10, ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)));
    glEnd();

    glutSwapBuffers();
}
/**
 * @brief Funkcja wyświetlająca menu opcji gry.
 */
void display3() {
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint texture;
    static char temp[10], temp2[10];
    //texture = LoadTexture("textures/settings.bmp");

    if(!Settings.fullscreen && !Settings.infiniteMode) {
        texture = LoadTexture("textures/settingsf0I0.bmp");
    }
    if(!Settings.fullscreen && Settings.infiniteMode) {
        texture = LoadTexture("textures/settingsf0I1.bmp");
    }
    if(Settings.fullscreen && !Settings.infiniteMode) {
        texture = LoadTexture("textures/settingsf1I0.bmp");
    }
    if(Settings.fullscreen && Settings.infiniteMode) {
        texture = LoadTexture("textures/settingsf1I1.bmp");
    }

    glBegin (GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth, 0.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth, Settings.resolutionHeight);
    glTexCoord2d(0.0, 1.0); glVertex2d(0.0, Settings.resolutionHeight);
    glEnd();

    texture = LoadTexture("textures/backButton.bmp");

    // back
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 0.0); glVertex2d(Settings.resolutionWidth*0.833, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 0.0); glVertex2d(Settings.resolutionWidth*0.931, Settings.resolutionHeight*0.755);
    glTexCoord2d(1.0, 1.0); glVertex2d(Settings.resolutionWidth*0.931, Settings.resolutionHeight*0.846);
    glTexCoord2d(0.0, 1.0); glVertex2d(Settings.resolutionWidth*0.833, Settings.resolutionHeight*0.846);
    glEnd();

    texture = LoadTexture("textures/applyButton.bmp");
    // apply
    glBegin(GL_QUADS);
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

/**
 * @brief Obsługuje wciskanie przycisków z klawiatury.
 * 
 * Funkcja obsługuje wciśnięcia przycisków z klawiatury, przesuwając tilesy odpowiednio.
 * @param key Wciśnięty klawisz.
 * @param x Współrzędna x ekranu.
 * @param y Współrzędna y ekranu.
 */
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

void mouseMenu(int button, int state, int x, int y) {
    static bool m;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // start
        if (x >= ((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10 && x <= ((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10 
            && y >= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-3*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40)) 
                && y <= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-2*(Settings.resolutionWidth/40))) {
            isGameStarted = true;
            m=false;
            srand(time(NULL));
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
            playBackgroundMusic("music//DS.wav");
        }
        // credits
        else if (x >= ((Settings.resolutionWidth)/2)-(Settings.resolutionWidth)/10 && x <= ((Settings.resolutionWidth)/2)+(Settings.resolutionWidth)/10 
        && y >= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-2*(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40))
        && y <= ((Settings.resolutionHeight)-(Settings.resolutionWidth/20)-(Settings.resolutionHeight/10)-(Settings.resolutionWidth/40))) {
            glutDisplayFunc(display3);
            glutMouseFunc(mouseSettings);
            glMatrixMode(GL_MODELVIEW);
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
    static int choiceCheck = 0;
    static bool fullscreenCheck = true;
    static bool infiniteCheck = false;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // back do menu
        if(x>=Settings.resolutionWidth*0.833 && x<=Settings.resolutionWidth*0.931 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            resolutionChoice=choiceCheck;
            Settings.fullscreen = fullscreenCheck;
            Settings.infiniteMode = infiniteCheck;
            glutDisplayFunc(display2);
            glutMouseFunc(mouseMenu);
        }

        // Apply
        if(x>=Settings.resolutionWidth*0.069 && x<=Settings.resolutionWidth*0.167 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            if(resolutionChoice!=choiceCheck && !Settings.fullscreen) {
                Settings.resolutionWidth=resolutions[resolutionChoice].width;
                Settings.resolutionHeight=resolutions[resolutionChoice].height;
                glutReshapeWindow(Settings.resolutionWidth, Settings.resolutionHeight);
                choiceCheck=resolutionChoice;
            }
            if(resolutionChoice!=choiceCheck && Settings.fullscreen) {resolutionChoice=0;}
            if(Settings.fullscreen && Settings.fullscreen!=fullscreenCheck) {
                glutFullScreen();
                Settings.resolutionWidth = resolutions[0].width;
                Settings.resolutionHeight = resolutions[0].height;
                resolutionChoice = 0;
                fullscreenCheck = Settings.fullscreen;
            }
            if(!Settings.fullscreen && Settings.fullscreen!=fullscreenCheck) {
                glutInitWindowPosition(0,0);
                glutReshapeWindow(Settings.resolutionWidth,Settings.resolutionHeight);
                fullscreenCheck = Settings.fullscreen;
            }
            if(Settings.infiniteMode!=infiniteCheck) {
                infiniteCheck = Settings.infiniteMode;
            }
            glLoadIdentity();
            gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
            glutDisplayFunc(display2);
            glutMouseFunc(mouseMenu);
        }

        // Res \/
        if(x>=Settings.resolutionWidth*0.079 && x<=Settings.resolutionWidth*0.108 
        && y>=Settings.resolutionHeight*0.139 && y<=Settings.resolutionHeight*0.204
        && resolutionChoice>=1) {
            resolutionChoice--;
        }

        // Res /\'
        if(x>=Settings.resolutionWidth*0.3389 && x<=Settings.resolutionWidth*0.368
        && y>=Settings.resolutionHeight*0.139 && y<=Settings.resolutionHeight*0.204
        && resolutionChoice<=5) {
            resolutionChoice++;
        }

        // fullscreen toggle
        if(x>=Settings.resolutionWidth*0.308 && x<=Settings.resolutionWidth*0.3448
        && y>Settings.resolutionHeight*0.311 && y<=Settings.resolutionHeight*0.3763) {
            if(Settings.fullscreen) { Settings.fullscreen = false; }
            else { Settings.fullscreen = true; }
        }

        // infinite
        if(x>=Settings.resolutionWidth*0.308 && x<=Settings.resolutionWidth*0.3448 
        && y>Settings.resolutionHeight*0.484 && y<=Settings.resolutionHeight*0.5494) {
            if(Settings.infiniteMode) { Settings.infiniteMode = false; }
            else { Settings.infiniteMode = true; }
        }

        // highscore reset
        if(x>=Settings.resolutionWidth*0.63 && x<=Settings.resolutionWidth*0.9209 
        && y>Settings.resolutionHeight*0.484 && y<=Settings.resolutionHeight*0.552) {
            fseek(file,0,SEEK_SET);
            fprintf(file,"%s","                           ");
            highscoreInt=0;
        }
    }
}

void mouseGame(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(x>=Settings.resolutionWidth*0.833 && x<=Settings.resolutionWidth*0.931 
        && y>=Settings.resolutionHeight*0.755 && y<=Settings.resolutionHeight*0.846) {
            playBackgroundMusic("music//menu.wav");
            glLoadIdentity();
            gluOrtho2D(0, Settings.resolutionWidth, Settings.resolutionHeight, 0);
            glEnable(GL_TEXTURE_2D);
            glutDisplayFunc(display2);
            glutSpecialFunc(NULL);
            glutMouseFunc(mouseMenu);
            isGameStarted = false;
            isBackgroundLoaded = false;
            score = 0;
        }
    }
}

/**
 * @brief Funkcja główna programu.
 * 
 * Funkcja główna programu wczytująca okno GLUT oraz pętlę główną.
 * Odpowiada za początkowe wejście do menu gry w trybie pełnoekranowym bazując na aktualnej rozdzielczości monitora.
 */
int main(int argc, char** argv) {
    playBackgroundMusic("music//menu.wav");
    isGameStarted = false;
    
    file = fopen("scoreboard.mp3","r+");
    fscanf(file,"%u",&highscoreInt);

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

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(Settings.resolutionWidth,Settings.resolutionHeight);
    glutCreateWindow("2048");
    glutFullScreen();
    Settings.fullscreen=true;
    Settings.infiniteMode=false;
    gluOrtho2D(0,Settings.resolutionWidth,Settings.resolutionHeight,0);
    glMatrixMode(GL_PROJECTION);
    glEnable( GL_TEXTURE_2D );
    glutDisplayFunc(display2);
    glutMouseFunc(mouseMenu);
    glutMainLoop();
    return 0;
}
