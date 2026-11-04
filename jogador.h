#ifndef JOGADOR_H
#define JOGADOR_H
#include "tiro.h"

class Jogador {
    float gX, gY;          // centro cabeza
    float gAngulo;         // giro
    float gAnguloArma;     // giro arma
    float gFase;           // pernas
    float gRaio;           //radio cabeza
public:
    Jogador(float x, float y, float raio);
    void Desenha();
    void Gira(float graus);                                 
    void Anda(float dist, float largura, float altura);     
    void MoveArma(float delta);                             
    Tiro Atira(float vel);                                  
    float ObtemX()    { return gX; }
    float ObtemY()    { return gY; }
    float ObtemRaio() { return gRaio; }
};

#endif
