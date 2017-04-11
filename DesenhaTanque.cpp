#include "DesenhaTanque.h"
#define RAD_TO_DEG (180.0/PI)

int keyStatus[256];
float mouseX = 0;
float mouseY = 0;
float oldX = 0;
float oldY = 0;
int tanque_jogador;
bool FLAG = true;
bool wLiberado = true;
bool sLiberado = true;
int camera = 1;
double camDist=10;
double camXYAngle=0;
double camXZAngle=0;
int lastX = 0;
int lastY = 0;
int buttonDown=0;
int vida = 3;
int qtdTanque;
float salva;
float help1;
float help2;

Coordenada *Ponto;

vector <Tanque> vetorTanques;
Tanque tanque;

bool colisao_Tanque(Tanque tanque)
{
	float d;
	for(int i = 0; i < vetorTanques.size(); i++)
	{
		if(vetorTanques[i].gX == tanque.gX || !vetorTanques[i].desenha)
			continue;
		
		d = sqrt(pow((vetorTanques[i].gX - tanque.gX), 2) + pow((vetorTanques[i].gY - tanque.gY), 2));
		if(d < tanque.raio + 20)
			return true;
		
	}
	return false;
}

void inicializa_tanque (float x, float y, float raio, bool jogador)
{
	tanque.gX = x - menor_x;
	tanque.gY = y - menor_y;
	tanque.esteira = 0;
	tanque.raio = raio;
	tanque.contrarioD = false;
	tanque.contrarioE = false;
	tanque.jogador = jogador;
	if(jogador)
		tanque.anguloCanhao = 180; //MUDEI DE -90 PARA 90 //TEM QUE GIRAR O TANQUE JUNTO COM O CANHAO, SENAO DA BOLOTAS
	else
	{
		tanque.anguloCanhao = -90;
		qtdTanque++;
		}
	tanque.anguloCanhao2 = 0;
	tanque.anguloTanque = -90;
	tanque.canhao_comprimento = raio;
	tanque.canhao_largura = raio/(raio*0.2);
	tanque.canhao_altura = raio/(raio*0.2);
	tanque.base_comprimento = raio;
	tanque.base_largura = raio;
	tanque.base_altura = raio/5;
	tanque.desenha = true;
	tanque.ladoD = 0;
	tanque.ladoE = 0;
	tanque.bateu = false;
	vetorTanques.push_back(tanque);
}

void teletransporte(float alturaR, float larguraR, float x, float y, float x2, float y2, Tanque *tanque)
{
	float a = (y - menor_y) - ((y - menor_y) + alturaR);
	float b = 0;
	float c = (x - menor_x + larguraR)*((y - menor_y) + alturaR) - (x - menor_x + larguraR)*(y - menor_y);
	float eq = tanque->gX * a + tanque->gY * b + c;
	float distanciaPR = fabs(eq)/sqrt(a*a);

	if(distanciaPR <= tanque->raio && !(tanque->gY >= ((y - menor_y) + alturaR) || tanque->gY <= (y - menor_y)))
	{
		tanque->gX = largura - tanque->gX + 50;
		return;
	}	
	
	a = (y2 - menor_y) - ((y2 - menor_y) + alturaR);
	b = 0;
	c = (x2 - menor_x)*((y2 - menor_y) + alturaR) - (x2 - menor_x)*(y2 - menor_y);
	eq = tanque->gX * a + tanque->gY * b + c;
	distanciaPR = fabs(eq)/sqrt(a*a);
	
	if(distanciaPR <= tanque->raio && !(tanque->gY >= ((y2 - menor_y) + alturaR) || tanque->gY <= (y2 - menor_y)))
	{
		tanque->gX = largura - tanque->gX - 50;
		return;
	}
	
}

void aponta_canhao (Tanque alvo, Tanque *atual)
{
	float x1, x2, y1, y2;
	x1 = alvo.gX;
	y1 = alvo.gY;
	
	x2 = atual->gX;
	y2 = atual->gY;
	
	atual->anguloCanhao = 180 + (atan2(y2 - y1, x2 - x1) * RAD_TO_DEG);
}

void atualizaCanhao (float gX, float gY, int mouseX, int mouseY, float *anguloCanhao, float *anguloCanhao2, bool jogador, float oldx, float oldy)
{

	for(int i = 0; i < vetorTanques.size(); i++)
	{
		if(!vetorTanques[i].jogador)
			aponta_canhao(vetorTanques[tanque_jogador], &vetorTanques[i]);
	}
}

void obstaculoFrente (Tanque tanque)
{
	if(tanque.gX > largura || tanque.gY > altura || tanque.gY < 0 || tanque.gX < 0)
		return;

	if(matriz[int(tanque.gY )][int (tanque.gX)] < tanque.raio/2.0)
	{
		wLiberado = false;
		sLiberado = true;
		return;
	}
	else
	{
		sLiberado = true;
		wLiberado = true;
	}
	
	return;
}

void obstaculoAtras (Tanque tanque)
{
	if(tanque.gX > largura || tanque.gY > altura || tanque.gY < 0 || tanque.gX < 0)
		return; 

	if(matriz[int(tanque.gY)][int (tanque.gX)] < tanque.raio/2)
	{
		sLiberado = false;
		wLiberado = true;
		return;
	}
	else
	{
		wLiberado = true;
		sLiberado = true;
	}
	
	return;
}

bool obstaculo (Tanque tanque)
{
	if(tanque.gX > largura || tanque.gY > altura || tanque.gY < 0 || tanque.gX < 0)
		return true;

	if(matriz[int(tanque.gY )][int (tanque.gX)] < tanque.raio/2)
		return true;
	
	return false;
}

void IA_adversarios()
{
	static int angulo = 0;
	for(int i = 0; i < vetorTanques.size(); i++)
	{
		if(vetorTanques[i].jogador)
			continue;

		if(obstaculo(vetorTanques[i]) || colisao_Tanque(vetorTanques[i]))
		{
			
			vetorTanques[i].bateu = true;
			vetorTanques[i].gY -= sin((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
			vetorTanques[i].gX -= cos((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
			vetorTanques[i].anguloTanque += angulo;
		
			if (vetorTanques[i].esteira > 5)
				vetorTanques[i].esteira = 0;
			
			if (vetorTanques[i].esteira < 0)
				vetorTanques[i].esteira = 5;
			
			else
				vetorTanques[i].esteira += 0.5;
		}
		else
		{
			if(vetorTanques[i].bateu)
			{
				vetorTanques[i].gY += sin((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
				vetorTanques[i].gX += cos((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
				
				if (vetorTanques[i].esteira > 5)
					vetorTanques[i].esteira = 0;

				if (vetorTanques[i].esteira < 0)
					vetorTanques[i].esteira = 5;

				else
					vetorTanques[i].esteira += 0.5;
				
				if(obstaculo(vetorTanques[i]))
				{
					vetorTanques[i].bateu = false;
					continue;
				}
			}
			else
			{
				vetorTanques[i].gY += sin((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
				vetorTanques[i].gX += cos((90 + vetorTanques[i].anguloTanque)*PI/180) * parametros.velocidade_tanque;
				
				if (vetorTanques[i].esteira > 5)
					vetorTanques[i].esteira = 0;

				if (vetorTanques[i].esteira < 0)
					vetorTanques[i].esteira = 5;

				else
					vetorTanques[i].esteira += 0.5;
				
				if(!obstaculo(vetorTanques[i]))
				{
					vetorTanques[i].bateu = false;
					continue;
				}
			}
		}
	
		if (vetorTanques[i].esteira > 5)
			vetorTanques[i].esteira = 0;
		if (vetorTanques[i].esteira < 0)
			vetorTanques[i].esteira = 5;
		else
			vetorTanques[i].esteira += 0.5;
		
		angulo += 5;

	}
}

void MoveCanhao (int x, int y)
{
	mouseX = x - 250;
	mouseY = y - 200;
	
	if(mouseY <= 250 && mouseY > 0)
	     vetorTanques[tanque_jogador].anguloCanhao2 = -(60 * mouseY)/250 + 60;
	     
	vetorTanques[tanque_jogador].anguloCanhao = (30  * -mouseX)/250 + 90;

	atualizaCanhao(vetorTanques[tanque_jogador].gX, vetorTanques[tanque_jogador].gY, x, y, &(vetorTanques[tanque_jogador].anguloCanhao), &(vetorTanques[tanque_jogador].anguloCanhao2), vetorTanques[tanque_jogador].jogador, oldX, oldY);
	oldX = x;
	oldY = y;
}

void DesenhaRetangulo(int comprimento, int largura, int altura,int chao, float R, float G, float B)
{
	
	GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1};
	GLfloat materialColor[] = { R, G, B, 1};
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	
       
     {
		glBegin(GL_QUADS);        // Draw The Cube Using quads   // Color Blue
			glNormal3f(0,0,-1);
			glVertex3f( largura/2, comprimento/2,chao);    // Top Right Of The Quad (Top)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2, comprimento/2,chao);    // Top Left Of The Quad (Top)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2, comprimento/2, altura + chao);    // Bottom Left Of The Quad (Top)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2, comprimento/2, altura + chao);    // Bottom Right Of The Quad (Top)
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,0,-1);
			glVertex3f( largura/2,-comprimento/2, altura + chao);    // Top Right Of The Quad (Bottom)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2, altura + chao);    // Top Left Of The Quad (Bottom)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2,chao);    // Bottom Left Of The Quad (Bottom)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2,-comprimento/2,chao);    // Bottom Right Of The Quad (Bottom)
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f( largura/2, comprimento/2, altura + chao);    // Top Right Of The Quad (Front)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2, comprimento/2, altura + chao);    // Top Left Of The Quad (Front)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2, altura + chao);    // Bottom Left Of The Quad (Front)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2,-comprimento/2, altura + chao);    // Bottom Right Of The Quad (Front)
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(0,-1,0);
			glVertex3f( largura/2,-comprimento/2,chao);    // Top Right Of The Quad (Back)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2,chao);    // Top Left Of The Quad (Back)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2, comprimento/2,chao);    // Bottom Left Of The Quad (Back)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2, comprimento/2,chao);    // Bottom Right Of The Quad (Back)
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(-1,0,0);
			glVertex3f(-largura/2, comprimento/2, altura + chao);    // Top Right Of The Quad (Left)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2, comprimento/2,chao);    // Top Left Of The Quad (Left)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2,chao);    // Bottom Left Of The Quad (Left)
			//glNormal3f(0,1,0);
			glVertex3f(-largura/2,-comprimento/2, altura + chao); 
		glEnd();
		glBegin(GL_QUADS);
			glNormal3f(1,0,0);
			glVertex3f( largura/2, comprimento/2, chao);    // Top Right Of The Quad (Right)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2, comprimento/2, altura + chao);    // Top Left Of The Quad (Right)
			//glNormal3f(0,1,0);
			glVertex3f( largura/2,-comprimento/2, altura + chao);    // Bottom Left Of The Quad (Right)
			//glNormal3f(0,1,0);
			glVertex3f(largura/2,-comprimento/2,chao);    // Bottom Right Of The Quad (Right)
		glEnd(); 
		glFlush();
	}
}

void desenhaRetangulo2d(struct retangulo retangulo)
{
     glColor3f(1 ,1 ,1);
  
     glBegin(GL_QUADS);
          glVertex2f(retangulo.ponto_x - menor_x  ,altura - (-retangulo.ponto_y + altura + menor_y));
          glVertex2f(retangulo.ponto_x - menor_x  , altura - (-retangulo.ponto_y -retangulo.altura + altura + menor_y));
          glVertex2f(retangulo.ponto_x + retangulo.largura - menor_x , altura - (-retangulo.ponto_y - retangulo.altura + altura + menor_y));
          glVertex2f(retangulo.ponto_x + retangulo.largura - menor_x ,altura - (altura - retangulo.ponto_y  + menor_y));
     glEnd();
}

void desenhaCirculo2d(int r, bool jogador)
{
	int i;
	float x, y;
	float theta = PI/r;
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	
	glPointSize(1);
	
	if(jogador)
	     glColor3f(0, 1, 0);
	else
	     glColor3f(1, 0, 0);
	x = r;
	y = 0;
	
	glBegin(GL_POLYGON);
	for(i = 0; i < 360; i++) 
	{
		glVertex2f(x, y);
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	}
	glEnd();
}

void desenhaRetangulo(struct retangulo retangulo, GLuint texture, GLuint teleporte)
{   
    {

    GLfloat materialEmission[] = { 1.0, 1.0, 1.0, 1};
    GLfloat materialColorA[] = { 0.2, 0.2, 0.2, 1};
    GLfloat materialColorD[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_specular[] = { 1.0, 0.0, 0.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glColor3f(1,1,1);

    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorA);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColorD);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    
    
    
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
//    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	if (retangulo.R > 0)
		glBindTexture (GL_TEXTURE_2D, teleporte);
	else
		glBindTexture (GL_TEXTURE_2D, texture);
    double textureS = 2;

	


	//glColor3f(1 ,1 ,1);
	//glBegin(GL_POLYGON); 
	
	
	
	//FRENTE
	glBegin(GL_POLYGON);
	glNormal3f(0,0,-1);
	glTexCoord2f (1, 0);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Top Right Of The Quad (Top)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 0);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Top Left Of The Quad (Top)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 1);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Bottom Left Of The Quad (Top)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Bottom Right Of The Quad (Top)
	glEnd();
	
	
	//ATRAS
	glBegin(GL_POLYGON);
	glNormal3f(0,0,-1);
	glTexCoord2f (1, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y, 33);    // Top Right Of The Quad (Bottom)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 1);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y, 33);    // Top Left Of The Quad (Bottom)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 0);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y,0);    // Bottom Left Of The Quad (Bottom)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 0);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y,0);    // Bottom Right Of The Quad (Bottom)
	glEnd();
	
	
	
	
	//TOPO
	glBegin(GL_POLYGON);
	glNormal3f(0,1,0);
	glTexCoord2f (1, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Top Right Of The Quad (Front)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 1);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Top Left Of The Quad (Front)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 0);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y, 33);    // Bottom Left Of The Quad (Front)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 0);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y, 33);    // Bottom Right Of The Quad (Front)
	glEnd();
	
	
	//EM BAIXO
	glBegin(GL_POLYGON);
	glNormal3f(0,-1,0);
	glTexCoord2f (1, 0);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y,0);    // Top Right Of The Quad (Back)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 0);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y,0);    // Top Left Of The Quad (Back)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 1);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Bottom Left Of The Quad (Back)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Bottom Right Of The Quad (Back)
	glEnd();
	
	
	
	//LADO 1
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glTexCoord2f (0, 1);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Top Right Of The Quad (Left)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 0);
	glVertex3f(retangulo.ponto_x - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Top Left Of The Quad (Left)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 0);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y,0);    // Bottom Left Of The Quad (Left)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 1);
	glVertex3f(retangulo.ponto_x - menor_x,retangulo.ponto_y  - menor_y, 33); 
	glEnd();
	
	
	//LADO 2
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glTexCoord2f (0, 0);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y,0);    // Top Right Of The Quad (Right)
	//glNormal3f(0,1,0);
	glTexCoord2f (0, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x, retangulo.ponto_y + retangulo.altura - menor_y, 33);    // Top Left Of The Quad (Right)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 1);
	glVertex3f( retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y, 33);    // Bottom Left Of The Quad (Right)
	//glNormal3f(0,1,0);
	glTexCoord2f (1, 0);
	glVertex3f(retangulo.ponto_x + retangulo.largura - menor_x,retangulo.ponto_y  - menor_y,0);    // Bottom Right Of The Quad (Right)
	
	
	glEnd(); 
	glFlush();
}
}


void DesenhaCirculo(int r, float R, float G, float B)
{
	int i;
	float x, y;
	float theta = PI/r;
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	
	glPointSize(1);
	//glColor3f(R, G, B);
	x = r;
	y = 0;
	
	//glBegin(GL_POLYGON);
	/*
	for(i = 0; i < 360; i++) 
	{
		glVertex2f(x, y);
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	}
	*/
	glutSolidSphere(r,20,20);
	//glEnd();
}

void desenhaCirculo(struct circulo circulo)
{
    int i;
    glColor3f(circulo.R, circulo.G, circulo.B);
    float x = circulo.raio;
	float y = 0; 
	float theta = 2*3.1415/circulo.raio; 
	float cose = cos(theta);
	float seno = sin(theta);
	float trans;
	
	glBegin(GL_POLYGON);
	for(int i = 0; i < 360; i++) 
	{ 
		glVertex2f(x + circulo.centro_x - menor_x, altura -(y + -circulo.centro_y + altura + menor_y));
		trans = x;
		x = cose * x - seno * y;
		y = seno * trans + cose * y;
	} 
	glEnd();
}

bool obstaculo_tiro (Tanque tanque, Coordenada *tiro, int pos)
{
	float d;
	if((tiro->xTiro > largura || tiro->yTiro > altura) || tiro->xTiro < 0 || tiro->yTiro < 0 || tiro->zTiro < 0 || tiro->zTiro > 55)
		return false;

	if(matriz[int(tiro->yTiro)][int (tiro->xTiro)] < 5 && tiro->zTiro <= 50)
		return true;
	
	for(int i = 0; i < vetorTanques.size(); i++)
	{
		if(tanque.gX == vetorTanques[i].gX || !vetorTanques[i].desenha)
			continue;
		
		d = sqrt(pow((vetorTanques[i].gX - tiro->xTiro), 2) + pow((vetorTanques[i].gY - tiro->yTiro), 2) +  pow((vetorTanques[i].raio/2 - tiro->zTiro), 2));
		if(d < vetorTanques[i].raio)
		{
			if(i == tanque_jogador && vida > 0)
			{
				vida--;
				if(vida == 0)
					vetorTanques[i].desenha = false;
				return true;
			}
			else
			{
				if (i != tanque_jogador && qtdTanque > 0)
				{
					qtdTanque--;
					cout << qtdTanque << "\n";
					if (qtdTanque == 0)
						vida = 4;
				}
				vetorTanques[i].desenha = false;
				return true;
			}
		}	
	}
	
	return false;
}

void DesenhaTiro(int base_altura, int base_largura, Tanque *tanque)
{
	float B;
	if (tanque->vetorTiros.size() == 0)
		return;

	for (int iterador = 0; iterador < tanque->vetorTiros.size(); iterador++)
	{
		// Atualiza tiro
		B = tanque->vetorTiros[iterador]->posicao;
		tanque->vetorTiros[iterador]->posicao += parametros.velocidade_tiro;
		B = tanque->vetorTiros[iterador]->posicao - B;
		tanque->vetorTiros[iterador]->xTiro -=  sin((tanque->vetorTiros[iterador]->angulo) * PI/180) * B;
		tanque->vetorTiros[iterador]->yTiro +=  cos((tanque->vetorTiros[iterador]->angulo) * PI/180) * B;
		tanque->vetorTiros[iterador]->zTiro += tan((tanque->vetorTiros[iterador]->angulo2) * PI/180) * B;
		
		if(obstaculo_tiro (*tanque, tanque->vetorTiros[iterador], iterador))
		{
			tanque->vetorTiros.erase(tanque->vetorTiros.begin() + iterador);
			break;
		}

		glPushMatrix();
				glTranslatef(tanque->vetorTiros[iterador]->xTanque, tanque->vetorTiros[iterador]->yTanque, tanque->vetorTiros[iterador]->zTiro);
				//glRotatef(tanque->vetorTiros[iterador]->angulo2, 1,0,0); //ADICIONADO
				//cout << tanque->vetorTiros[iterador]->angulo2 << "\n";
				glRotatef(tanque->vetorTiros[iterador]->angulo, 0,0,1);
				glRotatef(tanque->vetorTiros[iterador]->angulo2, 1,1,0);
				//cout << tanque->vetorTiros[iterador]->angulo << "\n";
				glTranslatef(0, tanque->vetorTiros[iterador]->posicao , 0);
				DesenhaCirculo(2, 1, 0, 0);
			glPopMatrix();
		
	}

}

void tiro_adversario(int value)
{
	glutTimerFunc(1/parametros.taxa_tiro, tiro_adversario, 0);
	for(int i = 0; i < vetorTanques.size(); i++)
	{
		if(vetorTanques[i].jogador)
			continue;
		
		if(!vetorTanques[i].desenha)
		     continue;
		
		Ponto = (Coordenada*)malloc(sizeof(Coordenada));

		Ponto->xTiro = vetorTanques[i].gX;
		Ponto->yTiro = vetorTanques[i].gY;

		Ponto->xTanque = vetorTanques[i].gX;
		Ponto->yTanque = vetorTanques[i].gY;
		Ponto->zTiro = vetorTanques[i].canhao_altura*1.5;
		
		Ponto->posicao = vetorTanques[i].canhao_comprimento - vetorTanques[i].base_altura/4.0;
		
		Ponto->angulo = vetorTanques[i].anguloCanhao - 90;
		vetorTanques[i].vetorTiros.push_back(Ponto);

	}
	glutPostRedisplay();
	
}

void DesenhaCanhao (GLfloat x, GLfloat y, int canhao_comprimento, int canhao_largura, int canhao_altura, int base_altura, float anguloCanhao, float anguloCanhao2)
{
	glPushMatrix(); //Canhão do tanque
		glRotatef(anguloCanhao - 90, 0,0,1);
		glRotatef(anguloCanhao2, 1,0,0);
		glTranslatef(0, canhao_comprimento/2, 0);
		DesenhaRetangulo(canhao_comprimento, canhao_largura, canhao_altura, base_altura, 1, 1, 0);
	glPopMatrix();
}

void DesenhaCanhao2 (GLfloat x, GLfloat y, int canhao_comprimento, int canhao_largura, int canhao_altura, int base_altura, float anguloCanhao)
{
	glPushMatrix(); //Canhão do tanque
		glRotatef(anguloCanhao - 90, 0,0,1);
		glTranslatef(0, canhao_comprimento/2, 0);
		DesenhaRetangulo(canhao_comprimento, canhao_largura, canhao_altura, base_altura, 1, 1, 0);
	glPopMatrix();
}

void DesenhaLinhaEsteira(int base_altura, int base_largura, float esteira, int ladoE, int ladoD, bool contrarioD, bool contrarioE)
{
	GLfloat materialColor[] = { 1, 1, 1, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, materialColor);
	glPushMatrix();
		//glColor3f(0, 0, 0);	// Linhas da esteira
		if (esteira == 0.5 || esteira == 1.5 || esteira == 2.5 || esteira == 3.5 || esteira == 4.5)
			salva = 1.5;
		else
			salva = 1;
		help1 = 4;
		help2 = 2.5;
		if (contrarioD)
		{
			for (float j = esteira; j > 0; j-= help1 - ladoD) 
			{
				glBegin(GL_LINES);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				//cout << " Esteira: " << esteira << "\n";
				glEnd();
			}
			
			for (float j = esteira; j > 0; j-= help1 - ladoD) 
			{
				glBegin(GL_LINES);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glEnd();
			}
			
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoD) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2, j);
				//cout << " J: " << base_altura/5 << "\n";
				glEnd();
			}
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoD) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, (base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, (base_altura + 10)/2, j);
				glEnd();
			}
		}
			
		}
		else
		{
			for (float j = esteira; j < (base_altura + 10) ; j+= help1 - ladoD) 
			{
				glBegin(GL_LINES);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glVertex3f((-base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glEnd();
			}
			
			for (float j = esteira; j < (base_altura + 10) ; j+= help1 - ladoD) 
			{
				glBegin(GL_LINES);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glVertex3f((-base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glEnd();
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoD) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glEnd();
			}
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoD) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, (base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, (base_altura + 10)/2, j);
				glEnd();
			}
			}
			
		}
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(0, 0, 0);	// Linhas da esteira
		if (contrarioE)
		{
			for (float j = base_altura + 10 - esteira; j > 0; j-= help1 - ladoE) 
			{
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glEnd();
			}
			
			for (float j = base_altura + 10 - esteira; j > 0; j-= help1 - ladoE) 
			{
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glEnd();
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoE) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glEnd();
			}
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoE) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, (base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, (base_altura + 10)/2, j);
				glEnd();
			}
			}
			
		}
		else
		{
			for (float j = esteira; j < base_altura + 10; j+= help1 - ladoE) {
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, base_altura/5 + 1);
				glEnd();
			}
			
			for (float j = esteira; j < base_altura + 10; j+= help1 - ladoE) {
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2 + j, 0 + 1);
				glEnd();
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoE) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, -(base_altura + 10)/2, j);
				glEnd();
			}
			}
			//ADD
			for (float j = salva; j <= base_altura/5 + 1; j += help2 + ladoE) 
			{
				for (int bn = 0; bn < 2; bn++){
				glBegin(GL_LINES);
				glVertex3f(-(base_largura/3.0)/2, (base_altura + 10)/2, j);
				glVertex3f((base_largura/3.0)/2, (base_altura + 10)/2, j);
				glEnd();
			}
		}
			
		}
	glPopMatrix();
}

void DesenhaEsteira (int base_largura, int base_comprimento, int altura ,float esteira, int ladoE, int ladoD, bool contrarioD, bool contrarioE)
{
	glPushMatrix();	//Esteira do tanque
		glTranslatef(base_largura * 2/3.0 , 01, 0);
		DesenhaRetangulo(base_comprimento + 10, base_largura/3.0, altura, 1, 0, 0, 1);
		DesenhaLinhaEsteira(base_largura, base_comprimento, esteira, ladoE, ladoD, contrarioD, contrarioE);
	glPopMatrix();
	
	glPushMatrix(); //Esteira do tanque

		glTranslatef(-base_largura * 2/3.0 , 0, 0);
		DesenhaRetangulo(base_comprimento + 10, -base_largura/3.0, altura, 1, 0, 0, 1);
		DesenhaLinhaEsteira(base_largura, base_comprimento, esteira, ladoE, ladoD, contrarioD, contrarioE);
	glPopMatrix();
}

void RasterChars(GLfloat x, GLfloat y, GLfloat z, const char * text, double r, double g, double b)
{
    //Push to recover original attributes
    glPushAttrib(GL_ENABLE_BIT);
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        //Draw text in the x, y, z position
        glColor3f(r,g,b);
        glRasterPos3f(x, y, z);
        const char* tmpStr;
        tmpStr = text;
        while( *tmpStr ){
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
            tmpStr++;
        }
    glPopAttrib();
}

void PrintText(GLfloat x, GLfloat y, const char * text, double r, double g, double b)
{
    //Draw text considering a 2D space (disable all 3d features)
    glMatrixMode (GL_PROJECTION);
    //Push to recover original PROJECTION MATRIX
    glPushMatrix();
        glLoadIdentity ();
        glOrtho (0, 1, 0, 1, -1, 1);
        RasterChars(x, y, 0, text, r, g, b);    
    glPopMatrix();
}

void atualizaCamera()
{
	float gx, gy, canhaoX, canhaoY, canhaoZ;
	float olhosX, olhosY, centroX, centroY;
	float anguloTanque = vetorTanques[tanque_jogador].anguloTanque;
	float anguloCanhao = vetorTanques[tanque_jogador].anguloCanhao;
	float anguloCanhao2 = vetorTanques[tanque_jogador].anguloCanhao2;

	 glViewport(0,0,500,500);
     glLoadIdentity();
	gluPerspective(60, 1, 0.1, 1400.0);
	if(vida == 3)
            PrintText(0.001, 0.05, "HEALTH POINT(S):  S2  S2  S2 ", 1,0,0);
    else if(vida == 2)
            PrintText(0.001, 0.05, "HEALTH POINT(S):  S2  S2", 1,0,0);
    else if(vida == 1)
                 PrintText(0.001, 0.05, "HEALTH POINT(S):  S2", 1,0,0);
              else if(vida == 0)
                   {
                   PrintText(0.44, 0.5, "YOU LOSE!", 1,0,0);
                   gluLookAt(0,0,0,0,0,0,0,0,0);
                   //acabou = true;
					return;
                   }
              else if (vida == 4)
              {
				  PrintText(0.44, 0.5, "YOU WIN!", 1,0,0);
                  gluLookAt(0,0,0,0,0,0,0,0,0);
                  return;
			  }
	
	gx = vetorTanques[tanque_jogador].gX;
	gy = vetorTanques[tanque_jogador].gY;
	canhaoZ = vetorTanques[tanque_jogador].canhao_altura/2;
	canhaoX = vetorTanques[tanque_jogador].canhao_largura/2;
	canhaoY = vetorTanques[tanque_jogador].canhao_comprimento;
	
	
	switch(camera)
	{
		case 1:
			PrintText(0.1, 0.1, "Camera 1", 0,1,0);
			olhosX = gx - cos((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			olhosY = gy - sin((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroX = gx + 500*cos((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroY = gy +  500*sin((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			canhaoZ = vetorTanques[tanque_jogador].base_altura * 3;
			//cout << olhosX << "    " << olhosY << "    " << centroX << "    " << centroY << "\n";
			gluLookAt(olhosX, olhosY, canhaoZ, centroX, centroY, canhaoZ ,0,0,1);
			//gluLookAt(gx , gy, 20, gx, gy, 5 ,0,0,1);
			break;
			
		case 2:
			PrintText(0.1, 0.1, "Camera 2", 0,1,0);
			olhosX = gx - 5*canhaoY *cos((180+anguloCanhao+ anguloTanque)*PI/180) * parametros.velocidade_tanque;
			olhosY = gy - 5*canhaoY *sin((180+anguloCanhao+ anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroX = gx - 20*canhaoY * cos((180 + anguloCanhao+ anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroY = gy - 20*canhaoY * sin((180 + anguloCanhao + anguloTanque)*PI/180) * parametros.velocidade_tanque ;
               canhaoZ = sin(vetorTanques[tanque_jogador].anguloCanhao2 * PI/180)* vetorTanques[tanque_jogador].canhao_comprimento + vetorTanques[tanque_jogador].base_altura + vetorTanques[tanque_jogador].canhao_altura;
			
			gluLookAt(olhosX, olhosY, canhaoZ, centroX, centroY, canhaoZ ,0,0,1);
			break;

		case 3:
 			PrintText(0.1, 0.1, "Camera 3", 0,1,0);
			glTranslatef(0 , 0, -45);
       		glRotatef(camXZAngle,1,0,0);
        	glRotatef(camXYAngle,0,1,0);
		        	olhosX = gx - cos((camXZAngle)*PI/180);
			olhosY = gy - sin((camXYAngle)*PI/180);
			centroX = gx;
			centroY = gy;
			gluLookAt(olhosX, olhosY , 15, centroX, centroY, 15,0,0,1);

			break;
	}

}

void atualizaCamera2()
{
	float gx, gy, canhaoX, canhaoY, canhaoZ;
	float olhosX, olhosY, centroX, centroY;
	float anguloTanque = vetorTanques[tanque_jogador].anguloTanque;
	float anguloCanhao = vetorTanques[tanque_jogador].anguloCanhao;

	 glViewport(0,500,500,200);
     glLoadIdentity();
	gluPerspective(60, 1, 0.1, 1400.0);
	
	
	gx = vetorTanques[tanque_jogador].gX;
	gy = vetorTanques[tanque_jogador].gY;
	canhaoZ = vetorTanques[tanque_jogador].canhao_altura/2;
	canhaoX = vetorTanques[tanque_jogador].canhao_largura/2;
	canhaoY = vetorTanques[tanque_jogador].canhao_comprimento;
	
	

			PrintText(0.1, 0.1, "Retrovisor", 0,1,0);
			olhosX = gx - 40*cos((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			olhosY = gy - 40*sin((90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroX = gx + 500*cos((-90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			centroY = gy +  500*sin((-90 + anguloTanque)*PI/180) * parametros.velocidade_tanque;
			canhaoZ = vetorTanques[tanque_jogador].base_altura * 3;
			//cout << olhosX << "    " << olhosY << "    " << centroX << "    " << centroY << "\n";
			gluLookAt(olhosX , olhosY, canhaoZ, centroX, centroY, canhaoZ ,0,0,1);
	

}

void atualizaCamera3()
{
     
	glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	glLoadIdentity();
	glViewport(500 - 125,0,125,125);
	gluOrtho2D(0, largura, altura, 0);
	
     glDisable(GL_LIGHTING);
     glDisable(GL_TEXTURE_2D);
     glEnable (GL_LINE_SMOOTH);
     glEnable (GL_BLEND);
     glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
     glLineWidth (1.5);
     //glPopMatrix();

}

void DesenhaTanque2d(Tanque *tanque)
{	
	if(tanque->desenha)
	{
	     glPushMatrix();
		     glTranslatef(tanque->gX, tanque->gY, 0);
		     glPushMatrix();
			     glRotatef(tanque->anguloTanque,0,0,1); //Base do tanque
                    desenhaCirculo2d(tanque->raio, tanque->jogador);
		     glPopMatrix();
	     glPopMatrix();
	     glFlush();
	}
}

void DesenhaTanque(Tanque *tanque)
{	

	if(tanque->desenha)
	{
		DesenhaTiro(tanque->base_altura, tanque->base_largura, tanque);
		glPushMatrix();
			glTranslatef(tanque->gX, tanque->gY, 0);
			glPushMatrix();
				glRotatef(tanque->anguloTanque,0,0,1); //Base do tanque
				if(tanque->jogador)
				{
					DesenhaRetangulo(tanque->base_comprimento, tanque->base_largura, tanque->base_altura, 1, 1,0, 0);
					DesenhaCirculo(tanque->base_largura/3, 0, 0, 1);
				}
				else
					DesenhaRetangulo(tanque->base_comprimento, tanque->base_largura, tanque->base_altura, 1, 0, 0, 1);
					DesenhaCirculo(tanque->base_largura/3, 0, 1, 0);
				DesenhaEsteira(tanque->base_comprimento, tanque->base_largura, tanque->base_altura, tanque->esteira, tanque->ladoE, tanque->ladoD, tanque->contrarioD, tanque->contrarioE);
			if(!tanque->jogador)
				glPopMatrix();
		
			atualizaCanhao(tanque->gX, tanque->gY, mouseX, mouseY, &(tanque->anguloCanhao), &(tanque->anguloCanhao2), tanque->jogador, oldX, oldY);
			//glPopMatrix();
			
			if(tanque->jogador)
				DesenhaCanhao(tanque->gX, tanque->gY, tanque->base_comprimento, tanque->canhao_largura, tanque->canhao_altura, tanque->base_altura, tanque->anguloCanhao, tanque->anguloCanhao2);
			else
				DesenhaCanhao2(tanque->gX, tanque->gY, tanque->base_comprimento, tanque->canhao_largura, tanque->canhao_altura, tanque->base_altura, tanque->anguloCanhao);
			glPopMatrix();
		glPopMatrix();
		glFlush();
	}
}

void keyPress(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'w':
        case 'W':
             keyStatus[(int)('w')] = 1;
             break;
        case 's':
        case 'S':
             keyStatus[(int)('s')] = 1;
             break;
        case 'a':
        case 'A':
             keyStatus[(int)('a')] = 1;
             break;
        case 'd':
        case 'D':
             keyStatus[(int)('d')] = 1;
             break;
	     
	case '1': 
		camera = 1;
		break;
		
	case '2': 
		camera = 2;
		break;
		
	case '3': 
		camera = 3;
		break;
	
        case 'q' :
		if (matriz != NULL)
		{
			for(int i = 0; i < largura; i++)
				if (matriz[i] != NULL)
					free(matriz[i]);
			free(matriz);
		}
				exit(0);
	}

    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void ResetKeyStatus()
{
    int i;
    //Initialize keyStatus
    for(i = 0; i < 256; i++)
       keyStatus[i] = 0; 
}

void MoveX(Tanque *tanque)
{
	tanque->gY -= sin((90 + tanque->anguloTanque)*PI/180) * parametros.velocidade_tanque;
	tanque->gX -= cos((90 + tanque->anguloTanque)*PI/180) * parametros.velocidade_tanque;

	
	if (tanque->esteira > 5)
		tanque->esteira = 0;
	
	if (tanque->esteira < 0)
		tanque->esteira = 5;
	
	else
		tanque->esteira += 0.5;
}

void MoveY(Tanque *tanque)
{
	tanque->gY += sin((90 + tanque->anguloTanque)*PI/180) * parametros.velocidade_tanque;
	tanque->gX += cos((90 + tanque->anguloTanque)*PI/180) * parametros.velocidade_tanque;

	
	if (tanque->esteira > 5)
		tanque->esteira = 0;
	
	if (tanque->esteira < 0)
		tanque->esteira = 5;
	
	else
		tanque->esteira += 0.5;
}

void idle ()
{
	IA_adversarios();
	
	if(wLiberado)
	{
		if (keyStatus[(int)('w')])
		{
			{
				MoveY(&(vetorTanques[tanque_jogador]));
				vetorTanques[tanque_jogador].ladoD = 0;
				vetorTanques[tanque_jogador].ladoE = 0;
				vetorTanques[tanque_jogador].contrarioE = false;
				vetorTanques[tanque_jogador].contrarioD = false;
				obstaculoFrente (vetorTanques[tanque_jogador]);
			}
		}
	}

	if(sLiberado)
	{
		if (keyStatus[(int)('s')])
		{
			{
				MoveX(&(vetorTanques[tanque_jogador]));
				vetorTanques[tanque_jogador].ladoD = 0;
				vetorTanques[tanque_jogador].ladoE = 0;
				vetorTanques[tanque_jogador].contrarioE = true;
				vetorTanques[tanque_jogador].contrarioD = true;
				obstaculoAtras (vetorTanques[tanque_jogador]);
			}
		}
	}
	
	if(sLiberado && wLiberado)
	{
		if (keyStatus[(int)('d')])
		{
			vetorTanques[tanque_jogador].anguloTanque +=(-1* parametros.velocidade_tanque );
			if(!keyStatus[(int)('a')])
				vetorTanques[tanque_jogador].ladoD = 0;
			else
				vetorTanques[tanque_jogador].ladoD = 0;
			if(!keyStatus[(int)('a')] && !keyStatus[(int)('w')] && !keyStatus[(int)('s')])
				vetorTanques[tanque_jogador].contrarioE = true;
			else
				vetorTanques[tanque_jogador].contrarioE = false;
		}

		if (keyStatus[(int)('a')])
		{
			vetorTanques[tanque_jogador].anguloTanque +=(1* parametros.velocidade_tanque);
				if(!keyStatus[(int)('d')])
				vetorTanques[tanque_jogador].ladoE = 0;
			else
				vetorTanques[tanque_jogador].ladoE = 0;
			if(!keyStatus[(int)('d')] && !keyStatus[(int)('w')] && !keyStatus[(int)('s')])
				vetorTanques[tanque_jogador].contrarioD = true;
			else
				vetorTanques[tanque_jogador].contrarioD = false;
		}
	}
	if (!keyStatus[(int)('d')])
		vetorTanques[tanque_jogador].ladoD = 0;

	if (!keyStatus[(int)('a')])
		vetorTanques[tanque_jogador].ladoE = 0;

	if(vetorTanques[tanque_jogador].anguloTanque >= 360)
		vetorTanques[tanque_jogador].anguloTanque -= 360;

	if(vetorTanques[tanque_jogador].anguloTanque <= -360)
		vetorTanques[tanque_jogador].anguloTanque += 360;
}

void mouse_camera(int x, int y)
{
    if (!buttonDown)
        return;
    
    camXYAngle += x - lastX;
    camXZAngle += y - lastY;
    
    camXYAngle = (int)camXYAngle % 360;
    camXZAngle = (int)camXZAngle % 360;
    
    lastX = x;
    lastY = y;

}
 
void GerenciaMouse(int button, int state, int x, int y)
{
	float i = vetorTanques[tanque_jogador].gX;
	float j = vetorTanques[tanque_jogador].gY;
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && vetorTanques[tanque_jogador].desenha)
	{	
		Ponto = (Coordenada*)malloc(sizeof(Coordenada));
		
		Ponto->xTiro = vetorTanques[tanque_jogador].gX;
		Ponto->yTiro = vetorTanques[tanque_jogador].gY;
		Ponto->posicao = vetorTanques[tanque_jogador].canhao_comprimento - vetorTanques[tanque_jogador].base_altura/4.0;;
		Ponto->xTanque = vetorTanques[tanque_jogador].gX;
		Ponto->yTanque = vetorTanques[tanque_jogador].gY;
		
		
		//Ponto->angulo = ((atan2(y  - vetorTanques[tanque_jogador].gY, x  - vetorTanques[tanque_jogador].gX)) * 360 /PI)/2  - 90;
		Ponto->angulo = vetorTanques[tanque_jogador].anguloCanhao - 180 + vetorTanques[tanque_jogador].anguloTanque + 90;
		Ponto->angulo2 = vetorTanques[tanque_jogador].anguloCanhao2;		
		Ponto->zTiro = sin((Ponto->angulo2) * PI/180)* vetorTanques[tanque_jogador].canhao_comprimento + vetorTanques[tanque_jogador].base_altura;
		vetorTanques[tanque_jogador].vetorTiros.push_back(Ponto);
			buttonDown = 0;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        lastX = x;
        lastY = y;
        buttonDown = 1;
	
     glutPostRedisplay();
}
}
