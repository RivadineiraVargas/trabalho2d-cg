#ifndef TIRO_H
#define TIRO_H

class Tiro {
    float gX, gY;          // posición
    float gDx, gDy;        // dirección
    float gVel;            // velocidad tiro
public:
    Tiro(float x, float y, float dx, float dy, float vel);   
    void Move(float dt);
    void Desenha(float raio);
    bool Valido(float largura, float altura);
    float ObtemX() { return gX; }
    float ObtemY() { return gY; }
};

#endif