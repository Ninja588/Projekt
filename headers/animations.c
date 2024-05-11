#include "animations.h"


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

void moveTilesLeft() {
    int d=0;
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
                if(!d) {
                    generateNewTile();
                    d=1;
                }
            }
        }
    }
    for (int i = 0; i < SIZE; i++) {
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
    yd=0;
    xd=-SPEED;
    glutPostRedisplay();
}

void moveTilesRight() {
    int d=0;
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
                if(!d) {
                    generateNewTile();
                    d=1;
                }
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
    yd=0;
    xd=SPEED;
    glutPostRedisplay();
}

void moveTilesUp() {
    int d=0;
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
                if(!d) {
                    generateNewTile();
                    d=1;
                }
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
    xd=0;
    yd=-SPEED;
    glutPostRedisplay();
}

void moveTilesDown() {
    int d=0;
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
                if(!d) {
                    generateNewTile();
                    d=1;
                }
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
    xd=0;
    yd=SPEED;
    glutPostRedisplay();
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