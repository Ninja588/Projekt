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

/**
 * @brief Rozmiar siatki gry.
 * Określa liczbę wierszy i kolumn w siatce gry.
 * Wartość ta kontroluje wielkość siatki gry, która jest kwadratowa.
 * Zwiększenie tej wartości zwiększy liczbę pól na siatce gry.
 */
#define SIZE 4 // rozmiar siatki

/**
 * @brief Siatka gry.
 * 
 * Dwuwymiarowa tablica przechowująca stan gry.
 * Wartości w siatce mogą być 0 (reprezentują puste miejsce) lub potęgi liczby 2 (2, 4, 8, 16, itd.).
 */
int grid[SIZE][SIZE]; // siatka

/**
 * @brief Liczba pustych miejsc na siatce gry.
 * 
 * Używane do kontroli, ile jeszcze miejsc na nowe klocki jest dostępnych.
 */
int emptySpaces; // puste miejsca (tiles z wartoscia '0')

// aktualizaowanie pustych miejsc na siatce
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

// generowanie nowego tile'sa w losowej pozycji na siatce 4x4 o wartosci 2 lub 4
/**
 * @brief Generuje nowy tile w losowej pozycji na siatce 4x4 o wartości 2 lub 4.
 * 
 * Funkcja generuje nowy tile (o wartości 2 lub 4) na losowej pozycji na siatce, 
 * jeśli są dostępne puste miejsca.
 */
void generateNewTile() {
    int value = rand() % 2 == 0 ? 2 : 4;
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

// przypisanie wartosci '0' do kazdego miejsca w siatce i wygenerowanie 2 poczatkowych tile'sow
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

// ruch w lewo
/**
 * @brief Przesuwa tilesy w lewo.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy, w lewo na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesLeft() {
    for (int i = 0; i < SIZE; i++) {
        int lastMerged = -1;

        // od lewej do prawej
        for (int j = 1; j < SIZE; j++) {
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

// ruch w prawo
/**
 * @brief Przesuwa tilesy w prawo.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w prawo na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesRight() {
    for (int i = 0; i < SIZE; i++) {
        int lastMerged = SIZE;

        for (int j = SIZE - 2; j >= 0; j--) {
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

// ruch w gore
/**
 * @brief Przesuwa tilesy w górę.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w górę na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesUp() {
    for (int j = 0; j < SIZE; j++) {
        int lastMerged = -1;

        for (int i = 1; i < SIZE; i++) {
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

// ruch w dol
/**
 * @brief Przesuwa tilesy w dół.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w dół na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesDown() {
    for (int j = 0; j < SIZE; j++) {
        int lastMerged = SIZE;

        for (int i = SIZE - 2; i >= 0; i--) {
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

// render siatki i tile'sow
/**
 * @brief Renderuje siatkę i tilesy.
 */
void renderGridAndTiles() {
    int temp = 0;
    int valueArray[4]; // tablica przechowywujaca wartosc tilesa do wypisania na nim
    int gridSize = SIZE; // rozmiar siatki (4x4)
    int tileSize = 100; // rozmiar tile (100x100 px)
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            int value = grid[i][j];
            float x = j * tileSize;
            float y = i * tileSize;
            switch(value) {
                case 0:
                    glColor3f(0.7, 0.7, 0.7);
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
            // if (value == 0) {
            //     glColor3f(0.7, 0.7, 0.7);
            // } else if (value == 2) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(1.0, 0.0, 0.0);
            // } else if (value == 4) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 1.0, 0.0);
            // } else if (value == 8) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.45 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 0.0, 1.0);
            // } else if (value == 16) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 1.0, 1.0);
            // } else if (value == 32) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 0.0, 1.0);
            // } else if (value == 64) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.4 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(1.0, 0.0, 1.0);
            // } else if (value == 128) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(1.0, 1.0, 1.0);
            // } else if (value == 256) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 2.0, 1.0);
            // } else if (value == 512) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.35 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(2.0, 0.0, 1.0);
            // } else if (value == 1024) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(2.0, 2.0, 1.0);
            // } else if (value == 2048) {
            //     glColor3f(0.0, 0.0, 0.0);
            //     glRasterPos2f(x + 0.3 * tileSize, y + 0.55 * tileSize);
            //     glColor3f(0.0, 2.0, 2.0);
            // }
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
    }
    glColor3f(0.0, 0.0, 0.0); // kolor linii
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
}

// funkcja wyswietlajaca
/**
 * @brief Funkcja wyświetlająca.
 * 
 * Funkcja wyświetla zawartość okna gry, renderując siatkę i tilesy.
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    renderGridAndTiles();
    glutSwapBuffers();
}

// funkcja sprawdzajaca, czy gracz przegral
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

// funkcja obslugujaca wciskanie przyciskow z klawiatury i obslugiwanie przegranej/wygranej
/**
 * @brief Obsługuje wciskanie przycisków z klawiatury oraz sprawdza przegraną i wygraną.
 * 
 * Funkcja obsługuje wciśnięcia przycisków z klawiatury, przesuwając tilesy odpowiednio.
 * Dodatkowo sprawdza, czy gracz przegrał lub wygrał, wyświetlając odpowiednie komunikaty.
 * @param key Wciśnięty klawisz.
 * @param x Współrzędna x ekranu.
 * @param y Współrzędna y ekranu.
 */
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

/**
 * @brief Funkcja odtwarzająca muzykę w tle.
 * 
 * Funkcja odtwarza muzykę z pliku o nazwie DS.wav
 */
void playBackgroundMusic() {
    PlaySound(TEXT("music//DS.wav"), NULL, SND_ASYNC | SND_LOOP);
}

/**
 * @brief Funkcja główna programu.
 * 
 * Funkcja główna programu, inicjalizuje siatkę gry, okno GLUT oraz pętlę główną.
 * @param argc Liczba argumentów wiersza poleceń.
 * @param argv Tablica argumentów wiersza poleceń.
 * @return Wartość zwracana przez funkcję main().
 */
int main(int argc, char** argv) {
    srand(time(NULL)); // generowanie seedu
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
    playBackgroundMusic();
    // for (int i = 0; i < SIZE; i++) {
    //     for (int j = 0; j < SIZE; j++) {
    //         int current = grid[i][j];
    //         printf("[%d][%d] = %d\n", i, j, current);
    //     }
    // }
    glutMainLoop();
    return 0;
}
