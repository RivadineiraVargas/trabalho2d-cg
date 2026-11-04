#include "barril.h"

Barril::Barril(float x, float y, int vidas, bool temInimigo)
{
    gX   = x;
    gY   = y;
    gVidas = vidas;
    gTemInimigo  = temInimigo;
    gAnguloArma = 0;

}

void Barril::Move(float dt, float vel)
{
    gY-= vel* dt;
}

void Barril::PerdeVida()
{
    gVidas--;
}

bool Barril::Atingido(Tiro &tiro, float largura, float altura)
{
    return tiro.ObtemX() >= gX - largura/2  &&
           tiro.ObtemX() <= gX + largura/2  &&
           tiro.ObtemY() >= gY              &&      // antes de suelo
           tiro.ObtemY() <= gY + altura;
}
