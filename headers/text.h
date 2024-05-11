/**
 * @file text.h
 * @brief Plik nagłówkowy zawierający funkcje odpowiedzialne za wypisywanie tekstu.
 * 
 */
#ifndef TEXT_H
#define TEXT_H

#include "text.c"
#include "globals.h"
#include <GL/glut.h>

/**
 * @brief Funkcja wypisująca tekst w typie 'int' na ekranie.
 * @param text Wartość liczbowa, która ma zostać wypisana
 * @param offsetWidth Wartość określająca pozycję w poziomie.
 * @param offsetHeightWartość określająca pozycję w pionie.
*/ 
void drawBitmapInt(unsigned int text, float offsetWidth, float offsetHeight);

/**
 * @brief Funkcja wypisująca tekst na ekranie.
 * @param text Tekst który ma zostać wypisany.
 * @param offsetWidth Wartość określająca pozycję w poziomie.
 * @param offsetHeight Wartość określająca pozycję w pionie.
*/
void drawBitmapString(char *text, float offsetWidth, float offsetHeight);

#endif // TEXT_H