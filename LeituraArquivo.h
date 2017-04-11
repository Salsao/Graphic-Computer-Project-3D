#ifndef LEITURAARQUIVO_H_
#define LEITURAARQUIVO_H_

#include "tinyxml.h"
#include "hexaRGB.h"
#include <string>
#include <vector>

#define Minimo(a, b) ((a) < (b) ? (a) : (b))
#define Maximo(a, b) ((a) > (b) ? (a) : (b))
#define CONFIG_XML "config.xml"

#define ARENA_DESCRIPTION 0
#define ARENA_MOBILITY 1
#define ARENA_DISTANCE_MAP 2

using namespace std;

struct arquivo
{
	string nome;
	string caminho;
};

struct parametros
{
	float velocidade_tiro;
	float taxa_tiro;
	float velocidade_tanque;
};

struct retangulo
{
	float ponto_x, ponto_y, altura, largura;
	float R, G, B;
};

struct circulo
{
	float centro_x, centro_y, raio;
	float R, G, B;
};

extern int **matriz;
extern float menor_x, menor_y;
extern int altura, largura;
extern int indice_tanque;
extern int indice_tlp ;
extern int indice_tlp2;

extern struct retangulo retangulo;
extern vector <struct retangulo> vetorRetangulos;

extern struct circulo circulo;
extern vector <struct circulo> vetorCirculos;

extern struct arquivo arquivos_entrada [3];

extern struct parametros parametros;

void Atributos_XML (TiXmlElement* elemento);

void arquivo_XML(void);

void Atributos_SVG(TiXmlElement* elemento);

void arquivo_SVG(void);

void arquivo_PGM(void);

void arquivo2_PGM(void);

void menorMaior (void);

#endif /* LEITURAARQUIVO_H_ */
