#ifndef BARRIL_H
#define BARRIL_H

#include "tiro.h"

class Barril {
    float gX, gY;          // posición (centro de la base)
    int   gVidas;          
    bool  gTemInimigo;     
    float gAnguloArma;

public:
    Barril(float x, float y, int vidas, bool temInimigo);    
    void Move(float dt, float vel);
    void PerdeVida();

    bool Atingido(Tiro &tiro, float largura, float altura);

    float ObtemX()     { return gX; }
    float ObtemY()     { return gY; }
    int   ObtemVidas() { return gVidas; }
    bool  TemInimigo()      { return gTemInimigo; }
    float ObtemAnguloArma() { return gAnguloArma; }
    void  DefineAnguloArma(float ang) { gAnguloArma = ang; }

};

#endif