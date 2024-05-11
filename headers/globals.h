/**
 * @file globals.h
 * @brief Plik nagłówkowy zawierający zmienne globalne programu.
 * 
 */
#ifndef GLOBALS_H
#define GLOBALS_H

#include "consts.h"
#include <stdio.h>
#include <stdbool.h>
#include "globals.c"

/**
 * @brief Siatka gry.
 * 
 * Dwuwymiarowa tablica przechowująca stan gry.
 * Wartości w siatce mogą być 0 (reprezentują puste miejsce) lub potęgi liczby 2 (2, 4, 8, 16, itd.).
 */
extern int grid[SIZE][SIZE]; // siatka

/**
 * @brief Liczba pustych miejsc na siatce gry.
 * 
 * Używane do kontroli, ile jeszcze miejsc na nowe klocki jest dostępnych.
 */
extern int emptySpaces; // puste miejsca (tiles z wartoscia '0')

/// @brief Parametr xd przesunięcia globalnego w macierzy transportu funkcji gltranslated.
extern int xd;
/// @brief Parametr yd przesunięcia globalnego w macierzy transportu funkcji gltranslated.
extern int yd;

/**
 * @brief Liczba określająca wielkość jednego bloku.
 */
extern int tileSize;

/**
 * @brief Zmienna przechowywująca aktualny wynik gracza.
 */
extern unsigned int score;

/**
 * @brief Zmienna przechowywująca największy wynik gracza.
 */
extern unsigned int highscoreInt;

/**
 * @brief Odwołanie się do pliku.
 * 
 * Używane jest do zczytywania wartości z pliku 'scoreboard.mp3'.
 * Zczytana wartość to największy wynik gracza.
 */
extern FILE *file;
/**
 * @brief Zmienna sprawdzjąca, czy gra jest wystartowana.
 */
extern bool isGameStarted;

/**
 * @brief Zmienna wybierania wyświetlanej rozdzielczosci (w opcjach) z tablicy rozdzielczości.
 */
extern unsigned int resolutionChoice;

/// @brief struktura zawierająca zmienne umożliwiające animacje blocków.
struct tilePos;

/**
 * @brief Struktura odpowiadająca za zmiane ustawień gry.
 * Potrafi zmienić rozdzielczość, włączyć lub wyłączyć tryb pełnoekranowy, włączyć tryb Infinite,
 * zmienić rozmiar siatki. 
 * */ 
struct Options;

/// @brief Struktura przechowywująca możliwe rozdzielczości gry.
struct resolution;

#endif // GLOBALS_H