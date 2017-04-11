#include "DesenhaTanque.h"
#include "LeituraArquivo.h"
#include "imageloader.h"

bool lido = true;
bool lido2 = true;
GLuint chao;
GLuint tijolo;
GLuint teto;
GLuint teleporte;
GLuint LoadTextureRAW( const char * filename );

void DisplayChao(GLuint texture)
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
    
    
    glBindTexture (GL_TEXTURE_2D, texture);
    double textureS = 1;
    glBegin (GL_QUADS);
        glNormal3f(0,1,0);
        glTexCoord2f (0, 0);
        glVertex3f (0, 0, -1);
        glNormal3f(0,1,0);
        glTexCoord2f (0, textureS);
        glVertex3f (0, altura, -1);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, textureS);
        glVertex3f (largura, altura, -1);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, 0);
        glVertex3f (largura, 0, -1);
    glEnd();
}

void DisplayTeto(GLuint texture)
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


    glBindTexture (GL_TEXTURE_2D, texture);
    double textureS = 1;
    glBegin (GL_QUADS);
        glNormal3f(0,1,0);
        glTexCoord2f (0, 0);
        glVertex3f (0, 0, 33);
        glNormal3f(0,1,0);
        glTexCoord2f (0, textureS);
        glVertex3f (0, altura, 33);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, textureS);
        glVertex3f (largura, altura, 33);
        glNormal3f(0,1,0);
        glTexCoord2f (textureS, 0);
        glVertex3f (largura, 0, 33);
    glEnd();
}

void desenha2d()
{
	
	for (int iterador = 0; iterador <= vetorCirculos.size() - 1; iterador++)
	{
		teletransporte(vetorRetangulos[indice_tlp].altura, vetorRetangulos[indice_tlp].largura, vetorRetangulos[indice_tlp].ponto_x, vetorRetangulos[indice_tlp].ponto_y, vetorRetangulos[indice_tlp2].ponto_x, vetorRetangulos[indice_tlp2].ponto_y, &(vetorTanques[iterador]));
		DesenhaTanque2d(&vetorTanques[iterador]);
	}
	
	for (int iterador = 0; iterador <= vetorRetangulos.size() - 1; iterador++)
		if(indice_tlp != iterador && indice_tlp2 != iterador)
			desenhaRetangulo2d(vetorRetangulos[iterador]);
			
	desenhaRetangulo2d(vetorRetangulos[indice_tlp]);
	desenhaRetangulo2d(vetorRetangulos[indice_tlp2]);
}

void desenha()
{

	
	     DisplayChao(chao);
	     DisplayTeto(teto);
	
	for (int iterador = 0; iterador <= vetorCirculos.size() - 1; iterador++)
	{
		teletransporte(vetorRetangulos[indice_tlp].altura, vetorRetangulos[indice_tlp].largura, vetorRetangulos[indice_tlp].ponto_x, vetorRetangulos[indice_tlp].ponto_y, vetorRetangulos[indice_tlp2].ponto_x, vetorRetangulos[indice_tlp2].ponto_y, &(vetorTanques[iterador]));
		DesenhaTanque(&vetorTanques[iterador]);
	}
	
	for (int iterador = 0; iterador <= vetorRetangulos.size() - 1; iterador++)
		if(indice_tlp != iterador && indice_tlp2 != iterador)
			desenhaRetangulo(vetorRetangulos[iterador], tijolo, teleporte);
			
	desenhaRetangulo(vetorRetangulos[indice_tlp], tijolo, teleporte);
	desenhaRetangulo(vetorRetangulos[indice_tlp2], tijolo, teleporte);
}

void DesenhaCena()
{	

     	GLfloat materialEmission[] = { 0.0, 0.0, 0.0, 1};
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1};
    GLfloat mat_shininess[] = { 100.0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmission);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	idle();	
	glMatrixMode(GL_PROJECTION); 
		
	atualizaCamera();
	 desenha();

	atualizaCamera2();
      desenha();
      

      atualizaCamera3();
      desenha2d();
      glEnable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D);
      

	glutSwapBuffers();

}

void idle1()
{
	 glutPostRedisplay();
}

void init(void)
{
	ResetKeyStatus();
	glShadeModel (GL_SMOOTH);
	glEnable(GL_LIGHTING); //DEIXA TUDO BRANCO BRANCO
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_LINE_SMOOTH);
	glEnable( GL_TEXTURE_2D );
	glLineWidth (1.5);
	chao = LoadTextureRAW( "chao.bmp" );
	tijolo = LoadTextureRAW( "mario.bmp" );
	teto = LoadTextureRAW( "ceu.bmp" );
	teleporte = LoadTextureRAW( "goku.bmp" );
	glEnable(GL_LIGHT0);
	
	//DIRECIONAL
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   
   
   //SPOT
   GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1_position[] = { -2.0, 2.0, 1.0, 1.0 };
GLfloat spot_direction[] = { -1.0, -1.0, 0.0 };

glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5);
glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5);
glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);

glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

glEnable(GL_LIGHT1);

}

int main(int argc, char *argv[])
{
	if(lido)
	{
		arquivo_XML();
		arquivo_SVG();
		arquivo_PGM();
		arquivo2_PGM();
		menorMaior();
		indice_tanque = tanque_jogador;
		lido = false;

		for (int iterador = 0; iterador <= vetorCirculos.size() - 1; iterador++)
		{	
			if(iterador == tanque_jogador)
				inicializa_tanque(vetorCirculos[iterador].centro_x, vetorCirculos[iterador].centro_y, vetorCirculos[iterador].raio, true);
			else
				inicializa_tanque(vetorCirculos[iterador].centro_x, vetorCirculos[iterador].centro_y, vetorCirculos[iterador].raio, false);
		}	
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 700);
	glutInitWindowPosition(0,0);
	glutCreateWindow("TRABALHO CURTO 4");
	glutDisplayFunc(idle1);
	
	glutKeyboardFunc(keyPress);
	glutIdleFunc(DesenhaCena);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(MoveCanhao);
	glutMotionFunc(mouse_camera);
	glutMouseFunc(GerenciaMouse);
	glutTimerFunc(10000, tiro_adversario, 0);

	init();
	
	glutMainLoop();

	return 0;
}


GLuint LoadTextureRAW( const char * filename )
{

    GLuint texture;
    
    Image* image = loadBMP(filename);



    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             image->width, image->height,  //Width and height
                             0,                            //The border of the image
                             GL_RGB, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image->pixels);               //The actual pixel data
    delete image;

    return texture;
}
