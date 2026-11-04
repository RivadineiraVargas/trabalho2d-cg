#include "jogador.h"
#include <GL/gl.h>
#include <math.h>

void desenhaRectangulo(float largura, float altura);   // definidas en main.cpp
void desenhaCirculo(float raio);
void desenhaPonta(float raio);

Jogador::Jogador(float x, float y, float raio)
{
    gX = x;
    gY = y;
    gRaio = raio;
    gAngulo = 0;            // mira arriba
    gAnguloArma = 0;        // arma derecha
    gFase = M_PI/2;         // reposo
}


void Jogador::Desenha()
{
    glPushMatrix();
        glTranslatef(gX, gY, 0);              // translacion centro cabeza
        glRotatef(gAngulo, 0, 0, 1);          // todo cuerpo

        glColor3f(0.0, 0.0, 0.0);             // pierna derecha
        glPushMatrix();
            glTranslatef(0.6*gRaio, 0, 0);
            desenhaRectangulo(0.5*gRaio, 1.8*gRaio * sin(gFase));
        glPopMatrix();

        glColor3f(0.0, 0.0, 0.0);             // pierna izquierda
        glPushMatrix();
            glTranslatef(-0.6*gRaio, 0, 0);
            desenhaRectangulo(0.5*gRaio, -1.8*gRaio * sin(gFase));
        glPopMatrix();

        glColor3f(1.0, 0.0, 0.0);             // arma
        glPushMatrix();
            glTranslatef(1.3*gRaio, -0.8*gRaio, 0);
            glRotatef(gAnguloArma, 0, 0, 1);
            desenhaRectangulo(0.6*gRaio, 3.5*gRaio);
        glPopMatrix();

        glColor3f(0.0, 1.0, 0.0);             // cabeza
        desenhaCirculo(gRaio);

        glColor3f(1.0, 1.0, 0.0);             // punta orientacion
        desenhaPonta(gRaio);
    glPopMatrix();
}

void Jogador::Gira(float graus)

{
    gAngulo+= graus;
}

void Jogador::Anda(float dist, float largura, float altura)     
{
    float rad = gAngulo * M_PI/180;
    gFase += 0.08 * fabs(dist);
    float novoX = gX + dist * -sin(rad);
    float novoY = gY + dist *  cos(rad);
    if (novoX >= gRaio && novoX <= largura - gRaio)    gX = novoX;
    if (novoY >= gRaio && novoY <= altura/2 - gRaio)   gY = novoY;
}

void Jogador::MoveArma(float delta)
{
    gAnguloArma += delta;
    if (gAnguloArma >  30) gAnguloArma =  30;
    if (gAnguloArma < -30) gAnguloArma = -30;
}

void RotatePoint(float x, float y, float angle, float &xOut, float &yOut)
{
    float rad = angle * M_PI/180;                 
    xOut = x * cos(rad) - y * sin(rad);           // x' = x·cos θ − y·sin θ
    yOut = x * sin(rad) + y * cos(rad);           // y' = x·sin θ + y·cos θ
}

Tiro Jogador::Atira(float vel)
{
    float armaRad = gAnguloArma * M_PI/180;
    float jogRad  = gAngulo     * M_PI/180;

    float puntaX = 1.3*gRaio + 3.5*gRaio * (-sin(armaRad));      
    float puntaY = -0.8*gRaio + 3.5*gRaio * ( cos(armaRad));

    float rx, ry;
    RotatePoint(puntaX, puntaY, gAngulo, rx, ry);    // orientacion con angulo
    float mundoX = gX + rx;                           // trasladar mundo para jugador
    float mundoY = gY + ry;


    return Tiro(mundoX, mundoY, -sin(jogRad + armaRad), cos(jogRad + armaRad), vel);
}