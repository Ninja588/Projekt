#include <GL/glut.h>
#include <stdbool.h>
#include <string.h>
#include<stdio.h>
/*
instrukcja:
F1-pauza
F2-strata hp
F3-odnowienie hp
F4-bron 1
F5-bron 2
F6-bron 3
*/

// Wymiary okna
const int windowWidth = 900;
const int windowHeight = 900;

//pauza
float pauza = 0.0f;

//pozycja HP
float hpX = 0.0f;
float hpY = 850.0f;
const float hpWidth = 150.0f;
const float hpHeight = 50.0f;
const float hpSpeed = 50.0f;
bool textActive = true;

// Pozycja gracza
float playerX = 400.0f;
const float playerY = 100.0f;
const float playerWidth = 50.0f;
const float playerHeight = 50.0f;
float playerSpeed = 20.0f;

// Pozycja pocisku
float bulletX = 400.0f;
float bulletY = 900.0f;
const float bulletWidth = 5.0f;
const float bulletHeight = 20.0f;
float bulletSpeed = 12.0f;
bool bulletActive = true;

// Pozycja pocisku wroga
float EbulletX = 425.0f;
float EbulletY = 900.0f;
const float EbulletWidth = 5.0f;
const float EbulletHeight = 20.0f;
float EbulletSpeed = 12.0f;
bool EbulletActive = true;

// Pozycja przeciwnika
float obstacleX = 400.0f;
const float obstacleY = 800.0f;
const float obstacleWidth = 50.0f;
const float obstacleHeight = 50.0f;
float obstacleSpeed = 1.5f;
bool obstacleActive = true;
int way = 0;

//punkty
int punkty = 0;

//bron
int bron = 1;

// Funkcja rysujoca gracza
void drawPlayer()
{
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(playerX, playerY);
    glVertex2f(playerX + playerWidth, playerY);
    glVertex2f(playerX + playerWidth, playerY + playerHeight);
    glVertex2f(playerX, playerY + playerHeight);
    glEnd();
}

// Funkcja rysujoca HP
void drawHP()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(hpX, hpY);
    glVertex2f(hpX + hpWidth, hpY);
    glVertex2f(hpX + hpWidth, hpY + hpHeight);
    glVertex2f(hpX, hpY + hpHeight);
    glEnd();
}

// Funkcja rysujoca przeszkody
void drawObstacle()
{
    if (obstacleActive)
        {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(obstacleX, obstacleY);
        glVertex2f(obstacleX + obstacleWidth, obstacleY);
        glVertex2f(obstacleX + obstacleWidth, obstacleY + obstacleHeight);
        glVertex2f(obstacleX, obstacleY + obstacleHeight);
        glEnd();
        }
}


// Funkcja rysujoca poscisku
void drawBullet()
{
    if (bulletActive)
        {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(bulletX, bulletY);
        glVertex2f(bulletX + bulletWidth, bulletY);
        glVertex2f(bulletX + bulletWidth, bulletY + bulletHeight);
        glVertex2f(bulletX, bulletY + bulletHeight);
        glEnd();
        }
}
// Funkcja rysujoca poscisku wroga
void drawEBullet()
{
    if (EbulletActive)
        {
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex2f(EbulletX, EbulletY);
        glVertex2f(EbulletX + EbulletWidth, EbulletY);
        glVertex2f(EbulletX + EbulletWidth, EbulletY + EbulletHeight);
        glVertex2f(EbulletX, EbulletY + EbulletHeight);
        glEnd();
        }
}

// Funkcja obsougujoca ruch pocisku
void updateBullet()
{
    bulletY += bulletSpeed*pauza;
    if(bulletX>obstacleX-15.0f && bulletX<obstacleX+50.0f && bulletY>obstacleY-14.0f && bulletY<obstacleY+5.0f)
        {
            if(hpX>-150.0f)
            {
                punkty += 100;
                bulletY = 900.f;
                printf("\nwynik: %d",punkty);
                EbulletSpeed+= 1.0f;
                obstacleSpeed+= 0.5f;
                playerSpeed-= 0.1f;
            }
        }
}

// Funkcja obsougujoca ruch pocisku wroga
void updateEBullet()
{
    EbulletY -= EbulletSpeed*pauza;
    if(EbulletX>playerX-15.0f && EbulletX<playerX+50.0f && EbulletY>playerY-14.0f && EbulletY<playerY+5.0f)
    {
        EbulletY = obstacleY;
        EbulletX = obstacleX+25.0f;
        if(hpX>-150.0f)
        {
            hpX -= hpSpeed;
        }
    }
    else if(EbulletY<=0.0f)
    {
        EbulletY = obstacleY;
        EbulletX = obstacleX+25.0f;
    }
}

void updateHP()
{
    if(EbulletY==playerY)
        if(EbulletX==playerX)
        hpX-=50.0f;
}

// Funkcja obsougujoca ruch przeszkód
void updateObstacle()
{
    if(way<1)
    {
        obstacleX += obstacleSpeed*pauza;
        if (obstacleX > 850.0f)
        {
            way = 1;
            obstacleActive = true;
        }
    }
    else
    {
    obstacleX -= obstacleSpeed*pauza;
    if (obstacleX < 0.0f)
        {
            way = 0;
            obstacleActive = true;
        }
    }
}
//
void updateGameOver()
{
    if(hpX==-150.0f)
    {
        pauza = 0.0f;
    }
}

// Funkcja obsougujoca ruch gracza
void updatePlayer(int key)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
        {
            playerX -= playerSpeed*pauza;
            break;
        }
        case GLUT_KEY_RIGHT:
        {
            playerX += playerSpeed*pauza;
            break;
        }
        case GLUT_KEY_UP:
        {
            switch(bron)
            {
                case 1:
                {
                    bulletSpeed = 12.0f;
                    if(bulletY>900.0f)
                    {
                        if(pauza == 1.0f)
                        {
                            bulletY =  playerY+50.0f;
                            bulletActive = true;
                            bulletX = playerX+25.0f;
                        }
                    }
                    break;
                }
                case 2:
                {
                    bulletSpeed = 120.0f;
                    if(bulletY>13500.0f)
                    {
                        if(pauza == 1.0f)
                        {
                            bulletY =  playerY+50.0f;
                            bulletActive = true;
                            bulletX = playerX+25.0f;
                        }
                    }
                    break;
                }
                case 3:
                {
                    break;
                }
            }
            if(bulletY>900.0f)
            {
                if(pauza == 1.0f)
                {
                    bulletY =  playerY+50.0f;
                    bulletActive = true;
                    bulletX = playerX+25.0f;
                }
            }
            break;
        }
        case GLUT_KEY_F1:
        {
            if(pauza > 0.0f)
            {
                pauza = 0.0f;
                break;
            }
            else if(pauza < 1.0f)
            {
                pauza = 1.0f;
                break;
            }
        }
        case GLUT_KEY_F2:
        {
            if(hpX>-150.0f)
            {
                hpX -= hpSpeed;
            }
            break;
        }
        case GLUT_KEY_F3:
        {
            if(hpX<0.0f)
            {
                hpX = 0.0f;
                punkty = 0;
                EbulletSpeed=12.0f;
                obstacleSpeed= 1.5;
                playerSpeed = 20.0f;
            }
            break;
        }

        case GLUT_KEY_F4:
        {
            bron = 1;
            break;
        }
        case GLUT_KEY_F5:
        {
            bron = 2;
            break;
        }
        case GLUT_KEY_F6:
        {
            bron = 3;
            break;
        }
        /*
        case GLUT_KEY_F7:
        {
            bulletSpeed += 1.0f;
            break;
        }
        */
    }
}
void wynik()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0f, 1.0f, 1.0f); // Ustawiamy kolor rysowania na biały

    glRasterPos2f(740.0f, 860.0f); // Ustawiamy pozycję tekstu
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'W');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'y');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'n');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'i');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'k');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ':');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/100000%10));
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/10000%10));
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/1000%10));
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/100%10));
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/10%10));
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,'0'+(punkty/1%10));
    glEnd;
}

void koniec()
{
    if(hpX==-150.0f)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f); // Ustawiamy kolor rysowania na biały

        glRasterPos2f(400.0f, 400.0f); // Ustawiamy pozycję tekstu
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'Z');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'B');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'I');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'J');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'I');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
        glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
        glutSwapBuffers();
    }
}

// Funkcja obsougujoca rysowanie sceny
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT);
    wynik();
    drawPlayer();
    drawObstacle();
    drawBullet();
    drawEBullet();
    drawHP();
    koniec();
    glutSwapBuffers();
}

// Funkcja obspugujpca zdarzenia klawiatury
void specialKeyPressed(int key, int x, int y)
{
    updatePlayer(key);
    glutPostRedisplay();
}

// Funkcja obsougujoca logikê gry
void update(int value)
{
    updateGameOver();
    updateObstacle();
    updateBullet();
    updateEBullet();
    updateHP();
    wynik();
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
    if(playerX>850.0f)
    {
        playerX = 850.0f;
    }
    else if(playerX<0.0f)
    {
        playerX = 0.0f;
    }
}


int main(int argc, char** argv)
{
    printf("F1-pauza\nF2-strata hp\nF3- odnowienie hp\nF4- predkosc enemy --\nF5- predkosc enemy ++\nF6- predkosc poscisku --\nF7- predkosc psocisku ++");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("space invaders");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glutDisplayFunc(drawScene);
    glutSpecialFunc(specialKeyPressed);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}