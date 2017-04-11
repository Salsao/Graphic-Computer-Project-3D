#include "LeituraArquivo.h"

int **matriz;
float menor_x, menor_y;
int altura, largura;
int indice_tanque = 0;
int indice_tlp = 0;
int indice_tlp2 = 0;

struct retangulo retangulo;
std::vector <struct retangulo> vetorRetangulos;

struct circulo circulo;
std::vector <struct circulo> vetorCirculos;

struct arquivo arquivos_entrada [3];

struct parametros parametros;

void Atributos_XML (TiXmlElement* elemento)
{
	if(elemento == NULL)
	{
		cout << "\nFalha ao abrir arquivo: Não tem elemento." << endl;
		exit(0);
	}
	
	for(int i = 0; elemento != NULL; elemento = elemento->NextSiblingElement(), i++)
	{
		switch(i)
		{
			case 0:
				arquivos_entrada[ARENA_DESCRIPTION].nome = (string)(elemento->Attribute("name")) + "." + (string)(elemento->Attribute("type"));
				arquivos_entrada[ARENA_DESCRIPTION].caminho = (string)elemento->Attribute("path") + arquivos_entrada[ARENA_DESCRIPTION].nome;
				break;
			
			case 1:
				arquivos_entrada[ARENA_MOBILITY].nome = (string)(elemento->Attribute("name")) + "." + (string)(elemento->Attribute("type"));
				arquivos_entrada[ARENA_MOBILITY].caminho = (string)elemento->Attribute("path") + arquivos_entrada[ARENA_MOBILITY].nome;
				break;
				
			case 2:
				arquivos_entrada[ARENA_DISTANCE_MAP].nome = (string)(elemento->Attribute("name")) + "." + (string)(elemento->Attribute("type"));
				arquivos_entrada[ARENA_DISTANCE_MAP].caminho = (string)elemento->Attribute("path") + arquivos_entrada[ARENA_DISTANCE_MAP].nome;
				break;
				
			case 3:
				parametros.velocidade_tiro = atof(elemento->Attribute("shootSpeed"));
				parametros.velocidade_tanque = atof(elemento->Attribute("tankSpeed"));
				parametros.taxa_tiro = atof(elemento->Attribute("shootsRate"));
				break;
		}
	}
}

void arquivo_XML()
{
	TiXmlDocument doc(CONFIG_XML);
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
		cout << "\n" << CONFIG_XML << endl;
	else
	{
		cout << "\nFalha ao abrir arquivo:" << CONFIG_XML << endl;
		exit(0);
	}

	TiXmlElement* root = doc.FirstChildElement();
	
	if(root == NULL)
	{
		cout << "\nFalha ao abrir arquivo: Nao tem elemento raiz." << endl;
		doc.Clear();
		exit (0);
	}
	
	for(TiXmlElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		string elemName = elem->Value();
		if(elemName == "inputFiles")
		{
			TiXmlElement* elemento = elem->FirstChildElement();
			Atributos_XML (elemento);
		}
	}
    doc.Clear();
}

void Atributos_SVG(TiXmlElement* elemento)
{
	int i = 0;
	int j = 0;
	
	if((string)elemento->Value() == "g")
	{	
		for( TiXmlElement* elemento1 = elemento->FirstChildElement(); elemento1 != NULL; elemento1 = elemento1->NextSiblingElement())
		{
			if((string)elemento1->Value() == "rect")
			{
				retangulo.ponto_x = atof(elemento1->Attribute("x"));
				retangulo.ponto_y = atof(elemento1->Attribute("y"));
				retangulo.largura = atof(elemento1->Attribute("width"));
				retangulo.altura = atof(elemento1->Attribute("height"));
				string palavra = elemento1->Attribute("style");
				palavra = palavra.substr(6, 6);
				
				hexacorRGB(palavra, &(retangulo.R), &(retangulo.G), &(retangulo.B));
				
				if(retangulo.R > 0.494 && retangulo.R < 0.501)
				{
					cout << retangulo.R*255 << " jsasadk" << j << endl;
					if(indice_tlp > 0)
						indice_tlp2 = j;
						
					if(indice_tlp == 0)	
						indice_tlp = j;		
				}
				
				j++;
				vetorRetangulos.push_back(retangulo);
			}
			
			if((string)elemento1->Value() == "circle")
			{	
				circulo.centro_x = atof(elemento1->Attribute("cx"));
				circulo.centro_y = atof(elemento1->Attribute("cy"));
				circulo.raio = atof(elemento1->Attribute("r"));
				
				string palavra =elemento1->Attribute("fill");
				palavra = palavra.substr(1, 6);
				
				hexacorRGB(palavra, &(circulo.R), &(circulo.G), &(circulo.B));
				
				vetorCirculos.push_back(circulo);

				if(circulo.R < 0.8)
					indice_tanque = i;
					
				i++;
			}
		}
	}
}

void arquivo_SVG()
{
	TiXmlDocument doc(arquivos_entrada[ARENA_DESCRIPTION].caminho.c_str());
	bool loadOkay = doc.LoadFile();
	
	if (loadOkay)
		cout << "\n" << arquivos_entrada[ARENA_DESCRIPTION].nome << endl;
	
	else
	{
		cout << "\nFalha ao abrir arquivo:" << arquivos_entrada[ARENA_DESCRIPTION].caminho.c_str() << endl;
		doc.Clear();
		exit(0);
	}
	
	TiXmlElement* root = doc.FirstChildElement();
	
	if(root == NULL)
	{
		cout << "\nFalha ao abrir arquivo: Nao tem elemento raiz." << endl;
		doc.Clear();
		exit(0);
	}
	
	for(TiXmlElement* elemento = root->FirstChildElement(); elemento != NULL; elemento = elemento->NextSiblingElement())
		Atributos_SVG(elemento);
	
    doc.Clear();
}

void arquivo_PGM(void)
{
	int k;
	ifstream  arquivo;
	string linha, px, py;
	arquivo.open(arquivos_entrada[ARENA_MOBILITY].caminho.c_str());
	bool load = arquivo.is_open();
	
	if (!load)
	{
		cout << "\nFalha ao abrir arquivo:" << arquivos_entrada[ARENA_MOBILITY].caminho.c_str() << endl;
		exit (0);
	}
	
	cout << "\n" << arquivos_entrada[ARENA_MOBILITY].nome << endl;
		
	getline (arquivo,linha);
	getline (arquivo,linha);
	getline (arquivo,linha);
	
	stringstream  stream(linha.c_str());
	stream >> px;
	stream >> py;
	
	largura = atof(px.c_str());
	altura = atof(py.c_str());
	arquivo.close();
}

void arquivo2_PGM(void)
{
	int k;
	ifstream  arquivo;
	string linha, px, py;
	arquivo.open(arquivos_entrada[ARENA_DISTANCE_MAP].caminho.c_str());
	bool load = arquivo.is_open();
	
	if (!load)
	{
		cout << "\nFalha ao abrir arquivo:" << arquivos_entrada[ARENA_DISTANCE_MAP].nome << endl;
		exit (0);
	}
	
	cout << "\n" << arquivos_entrada[ARENA_DISTANCE_MAP].nome << endl;
		
	getline (arquivo,linha);
	getline (arquivo,linha);
	getline (arquivo,linha);
	getline (arquivo,linha);
	
	matriz = (int **) malloc (sizeof(int*) * altura);
	if(matriz == NULL)
	{
		cout << "Memoria insuficiente para armazernar a matriz do arquivo .pgm" << endl;
		return;
	}
	
	for(int i = 0; i < altura; i++)
	{	
		matriz[i] = (int *) malloc (sizeof(int) * largura);
		if(matriz[i] == NULL)
		{
			cout << "Memoria insuficiente para armazernar a matriz do arquivo .pgm" << endl;
			return;
		}
		
		for(int j = 0; j < largura; j++)
		{
			getline (arquivo,linha);
		
			k = atoi(linha.c_str());
			matriz[i][j] = k;
		}
	}
	arquivo.close();
}

void menorMaior ()
{
	menor_x = 1000;
	menor_y = 1000;
	
	for (int iterador = 0; iterador < vetorRetangulos.size(); iterador++)
	{
		menor_x = Minimo(vetorRetangulos[iterador].ponto_x, menor_x);
		menor_y = Minimo(vetorRetangulos[iterador].ponto_y, menor_y);
	}
		
	for (int iterador = 0; iterador < vetorCirculos.size();iterador++)
	{
		menor_x = Minimo(vetorCirculos[iterador].centro_x, menor_x);
		menor_y = Minimo(vetorCirculos[iterador].centro_y, menor_y);
	}
}		
