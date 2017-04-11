/* Código achado na internet, autor desconhecido. */

#ifndef HEXARGB_H_
#define HEXARGB_H_

#include "hexaRGB.h"

// Bibliotecas essenciais para o funcionamento do arquivo TC2.cpp e TC2.h
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

static int Getdec(char hexchar);

float HexColorToFloat(string HexColor);

void hexacorRGB(string colorHex, float *R, float *G, float *B);

#endif /* HEXARGB */
