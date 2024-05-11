#include "text.h"

void drawBitmapInt(unsigned int text, float offsetWidth, float offsetHeight) {
    int temp = 0;
    int valueArray[10];
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,Settings.resolutionWidth, Settings.resolutionHeight, 0);
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(Settings.resolutionWidth*offsetWidth,Settings.resolutionHeight*offsetHeight);
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
    glLineWidth(2.0);
    glColor3f(1.0, 1.0, 1.0);
    glTranslated(Settings.resolutionWidth*offsetWidth,-Settings.resolutionHeight*offsetHeight,0);
    for(int i=0;i<strlen(text);i++) {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[i]);
    }
    glPopMatrix();
}