#include "tinyxml2.h"
using namespace tinyxml2;

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <ctype.h>
#include "tiro.h"
#include "barril.h"
#include "jogador.h"

int larguraArena = 200;
int alturaArena = 500;
float R = 50;

float faseInimigo = M_PI/2;
int ultimoMouseX = -1;

bool teclas[256] = {false};
float velocidadeGiro =90;
float velocidadeJogador =100;

float alturaBarril;
float larguraBarril;
int vidasBarril;
int nParaGanhar;
float velocidadeBarril;
float tempoBarril = 0;
int barrisDestruidos = 0;

bool jogoAcabou = false;
bool pausado = false;
bool ganhou = false;

char** argvGlobal;

float velocidadeTiro;
float tirosPorSegundo;
float raioInimigo;

std::vector<Tiro>tiros;

std::vector<Barril>barris;

Jogador jogador (0,0,50);

void desenhaRectangulo(float largura, float altura)
{

glBegin(GL_POLYGON);
   glVertex2f(-largura/2, 0.0);
   glVertex2f(largura/2, 0.0);
   glVertex2f(largura/2, altura);
   glVertex2f(-largura/2, altura);

   glEnd();
}

void desenhaCirculo(float raio)
{
   glBegin(GL_POLYGON);
      for (int i=0; i<30; i++){
         float angulo = i * (2*M_PI/30);
         glVertex2f(raio * cos(angulo),raio * sin(angulo));
      }
   glEnd();
}

void desenhaPonta(float raio)
{

   glBegin(GL_TRIANGLES);
   glVertex2f(0, 1.3*raio);
   glVertex2f(-0.3*raio, 0.8*raio);
   glVertex2f(0.3*raio, 0.8*raio);

   glEnd();
}

void escreveTexto(float x, float y, char* texto)
{
    glRasterPos2f(x, y);                              // posición donde empieza el texto
    for (int i = 0; texto[i] != '\0'; i++) {          // recorre letra por letra hasta el final
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, texto [i]);
    }
}

void reiniciar(){
   execv("/proc/self/exe", argvGlobal);
}

void desenhoInimigo(float r, float ang){

      glColor3f(0.0, 0.0, 0.0);
      glPushMatrix();
         glTranslatef(0.6*r, 0, 0);
         desenhaRectangulo(0.5*r, 1.8*r * sin(faseInimigo));
      glPopMatrix();//pierna derecha (adelante)

      glColor3f(0.0, 0.0, 0.0);
      glPushMatrix();
         glTranslatef(-0.6*r, 0, 0);
         desenhaRectangulo(0.5*r, -1.8*r * sin(faseInimigo));
      glPopMatrix();// pierna izquierda (atras)

      glColor3f(1.0, 0.0, 0.0);
      glPushMatrix();
         glTranslatef(1.3*r, -0.8*r, 0);
         glRotatef(ang, 0, 0, 1);
         desenhaRectangulo(0.6*r, 3.5*r);
      glPopMatrix();// arma                        

      glColor3f(0.6, 0.0, 0.8);
      desenhaCirculo(r);
}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   glColor3f (0.0, 0.0, 1.0);

   glBegin(GL_POLYGON);
      glVertex3f (0, 0, 0);
      glVertex3f (larguraArena, 0, 0);
      glVertex3f (larguraArena, alturaArena, 0);
      glVertex3f (0, alturaArena, 0);
   glEnd();

   jogador.Desenha();


      glColor3f(1.0, 1.0, 1.0);               // tiros blancos
      for (int i = 0; i < tiros.size(); i++) {
          tiros[i].Desenha(R);
      }
   /* Desenhar no frame buffer! */

   for (int i = 0; i < barris.size(); i++) {
      
      glPushMatrix();
         glTranslatef(barris[i].ObtemX(), barris[i].ObtemY(), 0);

         glColor3f(1.0, 0.5, 0.0);                               // naranja
         desenhaRectangulo(larguraBarril, alturaBarril);


         if (barris[i].TemInimigo()) {
            glPushMatrix();
               glTranslatef(0, alturaBarril/2, 0);
               glRotatef(180, 0, 0, 1);
               desenhoInimigo(raioInimigo, barris[i].ObtemAnguloArma());
            glPopMatrix();
         }           
         glColor3f(0, 0, 0);                                     // negro para el número
         char vidasTexto[10];
         sprintf(vidasTexto, "%d", barris[i].ObtemVidas());             // número
         escreveTexto(-5, alturaBarril/2 - 6, vidasTexto);    // meio barril
      glPopMatrix();
   }



   char texto[50];
   sprintf(texto, "Barris destruidos: %d", barrisDestruidos);
   glColor3f(1.0, 1.0, 1.0);
   escreveTexto(larguraArena - 220, alturaArena - 30, texto);

   if (jogoAcabou) {
       glColor3f(1.0, 1.0, 0.0);                                   // amarillo: se lee sobre el azul
       if (ganhou) escreveTexto(larguraArena/2 - 35, alturaArena/2, (char*)"Ganhou!");
       else        escreveTexto(larguraArena/2 - 35, alturaArena/2, (char*)"Perdeu!");
   }

   glutSwapBuffers(); //janela double buffer
}

void keypress(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 'a': case 'A':  teclas['a'] = true;  break;
        case 'd': case 'D':  teclas['d'] = true;  break;
        case 'w': case 'W':  teclas['w'] = true;  break;
        case 's': case 'S':  teclas['s'] = true;  break;
        case ' ':            pausado = !pausado;  break;   
        case 'r': case 'R':  reiniciar();         break;
        case 27:             exit(0);
    }
   
}

void keyboardUp(unsigned char key, int x, int y)

{
   teclas [tolower(key)]=false;

}

void passiveMotion(int x, int y)
{
    if (ultimoMouseX != -1) {
        int dx = x - ultimoMouseX;    
        jogador.MoveArma(-dx * 0.5);
    }
    ultimoMouseX = x;
}

void criarBarril()
   {  
      float x = larguraBarril/2 + rand() % (int)(larguraArena - larguraBarril);
      Barril b(x, alturaArena, vidasBarril, rand()%2==0);
      barris.push_back(b);
}

void mouse(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !jogoAcabou)
   {
      tiros.push_back(jogador.Atira(velocidadeTiro));
   }
}


void idle (void){

 static int tempoAnterior = glutGet(GLUT_ELAPSED_TIME);
   int tempoAtual = glutGet(GLUT_ELAPSED_TIME);
    float dt = (tempoAtual - tempoAnterior) / 1000.0;
    tempoAnterior = tempoAtual;
    if (jogoAcabou) return;
    if (pausado) return;
    faseInimigo += 8*dt;

    if (teclas['a'] ) jogador.Gira( velocidadeGiro * dt);    // antihorario
    if (teclas['d'] ) jogador.Gira(-velocidadeGiro * dt);    // horario
    if (teclas['w'] ) jogador.Anda( velocidadeJogador * dt, larguraArena, alturaArena);
    if (teclas['s'] ) jogador.Anda(-velocidadeJogador * dt, larguraArena, alturaArena);

      for (int i = 0; i < tiros.size(); i++) {
        tiros[i].Move(dt);
        if (!tiros[i].Valido(larguraArena, alturaArena)) {    // salio?
            tiros.erase(tiros.begin() + i);                    // borrarlo
            i--;                                               
        }
    }

      for (int i = 0; i < barris.size(); i++) {
        barris[i].Move(dt, velocidadeBarril);      // velocidad×tiempo,  abajo
    }

    tempoBarril += dt;               
    if (tempoBarril >= 3) {          
        criarBarril();                          
        tempoBarril = 0;                  
    }
    glutPostRedisplay();

    for (int i = 0; i < tiros.size(); i++) {
        for (int j = 0; j < barris.size(); j++) {
            if (barris[j].Atingido(tiros[i], larguraBarril, alturaBarril)) {
                 barris[j].PerdeVida();
                 if (barris[j].ObtemVidas() <= 0) {
                    barris.erase(barris.begin()+j);        // borrar el barril j
                    barrisDestruidos++;                        // contar uno más
                    if (barrisDestruidos >= nParaGanhar) {     // meta XML
                        jogoAcabou = true;
                        ganhou = true;                         // gano
                    }
                    printf("destruidos: %d\n", barrisDestruidos);

                }
                tiros.erase(tiros.begin() + i);        // borrar vida barril
                i--;
                break;
            }
        }
    }
    for (int j = 0; j < barris.size(); j++) {
        float cercaX = jogador.ObtemX();
        float cercaY = jogador.ObtemY();


        if (cercaX < barris[j].ObtemX() - larguraBarril/2) cercaX = barris[j].ObtemX() - larguraBarril/2;
        if (cercaY < barris[j].ObtemY())                cercaY = barris[j].ObtemY();
        if (cercaX > barris[j].ObtemX() + larguraBarril/2) cercaX = barris[j].ObtemX() + larguraBarril/2;
        if (cercaY > barris[j].ObtemY() + alturaBarril) cercaY = barris[j].ObtemY() + alturaBarril;   
        float dx = jogador.ObtemX()- cercaX;
        float dy = jogador.ObtemY() - cercaY;
        if (dx*dx + dy*dy < R*R) {
            jogoAcabou=true;
            ganhou = false;
            printf("morreu!\n");
        }
        if (barris[j].ObtemY() < 0) {     // barril fondo
            jogoAcabou = true;
            ganhou = false;
        }

    }
    

   }
void init (void) 
{
  
  glClearColor (0.0, 0.0, 0.0, 0.0);

  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, larguraArena, 0, alturaArena, -1, +1);
  glMatrixMode(GL_MODELVIEW);     // matriz objetos
  glLoadIdentity();
}

int main(int argc, char** argv)
{
    argvGlobal=argv;
    srand(time(0));
    XMLDocument doc;
    doc.LoadFile("configuracoes.xml");
    XMLElement* jogo  = doc.FirstChildElement("jogo");
    XMLElement* arena = jogo->FirstChildElement("arena");   

    larguraArena = arena->IntAttribute("largura");            
    alturaArena  = arena->IntAttribute("altura");

    XMLElement* jogadorXml = jogo->FirstChildElement("jogador");
    R                 = jogadorXml->FloatAttribute("raioCabeca");
    velocidadeJogador = jogadorXml->FloatAttribute("velocidade");

    XMLElement* barril = jogo->FirstChildElement("barril");
    alturaBarril     = barril->FloatAttribute("altura");
    larguraBarril =  barril->FloatAttribute("largura");
    vidasBarril     = barril->IntAttribute("numeroTiros");
    nParaGanhar =  barril->IntAttribute("nParaGanhar");
    velocidadeBarril = barril->FloatAttribute("velocidade");

    XMLElement* inimigo = jogo->FirstChildElement("inimigo");
    velocidadeTiro  = inimigo->FloatAttribute("velocidadeTiro");
    tirosPorSegundo = inimigo->FloatAttribute("tirosPorSegundo");
    raioInimigo = inimigo->FloatAttribute("raioCabeca"); 

    jogador = Jogador(larguraArena/2,  2*R, R);
    criarBarril();


    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (larguraArena, alturaArena); 
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Trabalho 2D");
    init ();
    glutDisplayFunc(display); 

    glutKeyboardFunc(keypress);
    glutKeyboardUpFunc(keyboardUp);
    glutIdleFunc(idle);
    glutMotionFunc(passiveMotion);
    glutPassiveMotionFunc(passiveMotion);

    glutMouseFunc(mouse);

    glutMainLoop();

    return 0;
}
