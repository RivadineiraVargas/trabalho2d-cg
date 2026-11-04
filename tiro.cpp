#include "tiro.h"
#include <GL/gl.h>

void desenhaCirculo(float raio);     

Tiro::Tiro(float x, float y, float dx, float dy, float vel)
{
    gX   = x;
    gY   = y;
    gDx  = dx;
    gDy  = dy;
    gVel = vel;   
}

void Tiro::Move(float dt)
{
    gX += gVel * dt * gDx;
    gY += gVel * dt * gDy;
}

void Tiro::Desenha(float raio)
{
    glPushMatrix();
        glTranslatef(gX, gY, 0);
        desenhaCirculo(raio/5);
    glPopMatrix();
}

bool Tiro::Valido(float largura, float altura)
{
    return gX >= 0 && gX <= largura && gY >= 0 && gY <= altura;
}