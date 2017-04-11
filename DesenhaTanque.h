#ifndef DESENHATANQUE_H_
#define DESENHATANQUE_H_

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include "LeituraArquivo.h"

#define radiusWheel 30
#define PI 3.14159265

struct Coordenada
{
	float xTiro;
	float yTiro;
	float zTiro;
	float posicao;
	float xTanque;
	float yTanque;

	float angulo;
	float angulo2;
};

struct Tanque
{
	float anguloCanhao;
	float anguloCanhao2;
	float anguloTanque;
	float gY;
	float gX;
	float esteira;
	float ladoD;
	float ladoE;
	float raio;
	bool contrarioD;
	bool contrarioE;
	int canhao_comprimento;
	int canhao_largura;
	int canhao_altura;
	int base_altura;
	int base_largura;
	int base_comprimento;
	bool jogador;
	bool desenha;
	bool bateu;
	vector <Coordenada*> vetorTiros;
};

extern int keyStatus[256];
extern int camera;
extern float mouseX;
extern float mouseY;
extern int tanque_jogador;
extern vector<Tanque> vetorTanques;

void inicializa_tanque (float x, float y, float raio, bool jogador);

void mouse2 (int valor);

void MoveCanhao (int x, int y);

void tiro_adversario(int value);

void teletransporte(float alturaR, float larguraR, float x, float y, float x2, float y2, Tanque *tanque);

bool obstaculo (Tanque tanque, char ws);

void DesenhaRetangulo(int comprimento, int largura, int altura, int chao, float R, float G, float B);

void desenhaRetangulo(struct retangulo retangulo, GLuint texture, GLuint teleporte);

void DesenhaCirculo(int r, float R, float G, float B);

void DesenhaTanque2d(Tanque *tanque);

void DesenhaCoordenada();

void desenhaRetangulo2d(struct retangulo retangulo);

void desenhaCirculo2d(int r, bool jogador);

void desenhaCirculo(struct circulo circulo);

void atualizaCamera();

void atualizaRetrovisor();

void atualizaCamera2();

void atualizaCamera3();

void mouse_camera(int x, int y);
void DesenhaTiro(int base_altura, int base_largura, vector <Coordenada> vetorPontos);

void DesenhaCanhao (GLfloat x, GLfloat y, int canhao_comprimento, int canhao_largura, int canhao_altura, int base_altura, float anguloCanhao, float anguloCanhao2);

void DesenhaCanhao2 (GLfloat x, GLfloat y, int canhao_comprimento, int canhao_largura, int canhao_altura, float anguloCanhao);

void DesenhaLinhaEsteira(int base_altura, int base_largura, float esteira, int ladoE, int ladoD, bool contrarioD, bool contrarioE);

void DesenhaEsteira (int base_largura, int base_comprimento, int altura ,float esteira, int ladoE, int ladoD, bool contrarioD, bool contrarioE);

void DesenhaTanque(Tanque *tanque);

void keyPress(unsigned char key, int x, int y);

void keyup(unsigned char key, int x, int y);

void ResetKeyStatus();


void idle ();

void GerenciaMouse(int button, int state, int x, int y);

#endif /* DESENHATANQUE_H_ */
