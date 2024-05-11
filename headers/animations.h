/**
 * @file animations.h
 * @brief Plik nagłówkowy zawierający funkcje odpowiedzialne za animacje.
 * 
 */
#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "consts.h"
#include "globals.h"
#include "animations.c"

/// @brief Struktura poprawiająca czytelność kodu fukcji na aktualizowanie ilość pustych miejsc.
struct tile;
/**
 * @brief Generuje nowy tile w losowej pozycji na siatce 4x4 o wartości 2 lub 4.
 * 
 * Funkcja generuje nowy tile (o wartości 2 lub 4) na losowej pozycji na siatce, 
 * jeśli są dostępne puste miejsca.
 */
void generateNewTile();
/**
 * @brief Resetowanie tablicy animacji.
 * 
 * Jako, że 0 może być realnym koordynatem, sprawdzanie czy dany block ma pozostać w bezruchu wykonuje się za pomocą wartości: -1.
 * Funcja ta służy do 'wyzerowania' owej tablicy, czyli wypełnienia odpowiednich zmiennych wartością: -1.
 */
void resetanimationPos();
/**
 * @brief Przesuwa tilesy w lewo.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy, w lewo na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesLeft();
/**
 * @brief Przesuwa tilesy w prawo.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w prawo na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesRight();
/**
 * @brief Przesuwa tilesy w górę.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w górę na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesUp();
/**
 * @brief Przesuwa tilesy w dół.
 * 
 * Funkcja przesuwa wszystkie niepuste tilesy w dół na siatce, 
 * łącząc te same wartości, jeśli to możliwe.
 */
void moveTilesDown();
/**
 * @brief Funkcja wywołująca się 60 razy na sekundę.
 * 
 * Funkcja, która 60 razy na sekundę przesuwa odpowiednie blocki umożliwiając przy tym animacje.
 */
void timer();



#endif // ANIMATIONS_H