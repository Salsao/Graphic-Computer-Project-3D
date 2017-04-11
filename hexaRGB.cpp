/* Código achado na internet e modificado de acordo com as necessidades do trabalho, autor desconhecido. */


#include "hexaRGB.h"

static int Getdec(char hexchar)
{
	if ((hexchar >= '0') && (hexchar <= '9')) return hexchar - '0';
	if ((hexchar >= 'A') && (hexchar <= 'F')) return hexchar - 'A' + 10;
	if ((hexchar >= 'a') && (hexchar <= 'f')) return hexchar - 'a' + 10;

	return -1; // Wrong character
}

float HexColorToFloat(string HexColor)
{
	float IntColor;
	IntColor = (Getdec(HexColor[0]) * 16) + Getdec(HexColor[1]);
	IntColor = IntColor / 255.0;
	if (IntColor < 0)
	{
		cout << "Erro na leitura de cor -> " << HexColor << endl;
		cout << "Por padrao sera setado para cor Branca -> 1." << endl;
		return 1;
	}
	return IntColor;
}


//Recebe string ou char* no formarto hexadecimal sem o "#". Ex.: FFAABB ou ffaabb.
void hexacorRGB(string colorHex, float *R, float *G, float *B)
{
	string Rh = colorHex.substr(0, 2); istringstream(Rh) >> hex >> *R;
	string Gh = colorHex.substr(2, 2); istringstream(Gh) >> hex >> *G;
	string Bh = colorHex.substr(4, 2); istringstream(Bh) >> hex >> *B;
	*R = HexColorToFloat(Rh);
	*G = HexColorToFloat(Gh);
	*B = HexColorToFloat(Bh);
}
