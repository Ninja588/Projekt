#include "score.h"
#include "globals.h"

void scoreHandler(int mergedValue) {
    if(isGameStarted) {
        score+=mergedValue*2;
        if(score>highscoreInt) {
            highscoreInt=score;
            fseek(file,0,SEEK_SET);
            fprintf(file,"%u",highscoreInt);
        }
    }
    else score = 0; 
}