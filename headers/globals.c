#include "globals.h"

int grid[SIZE][SIZE];

int emptySpaces;

int xd;

int yd;

int tileSize = 800/SIZE;

unsigned int score = 0;

unsigned int highscoreInt = 0;

FILE *file;

bool isGameStarted;

unsigned int resolutionChoice;

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
    const int size;
    bool infiniteMode;
}Settings;

struct resolution {
    int width;
    int height;
}resolutions[7];
