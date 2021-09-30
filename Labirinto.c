#include<time.h>
#include <stdio.h>
#include <gl/glut.h>
#include <windows.h>
#include <stdlib.h>
#define iniX 100
#define iniY 1
#define esp 5
#define win 200
#define quad 9

float a=1,b=1,c=1,d=0;
int life;
void DesenhaTextoStroke(void *font, char *string);
void DesenhaTexto(void *font, char *string);
void GerenciaMouse(int button, int state, int x, int y);
int venceu();
void colisao();
void Desenha();
void Inicializa();
void Teclado (unsigned char key, int x, int y);
void AlteraTamanhoJanela(GLsizei w, GLsizei h);
void tecladoSpec (int key, int x, int y);
void desenhaLinhas();
void desenhaObjeto();
GLint xf, yf;
GLint view_w, view_h;
int MAP[200][200];
int desenha=0;
int random[9], rand2;
int main(void)
{
	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(250,20); 
	glutInitWindowSize(800,600); 
	glutCreateWindow("Maze game");
	glutDisplayFunc(Desenha);
//  	glutReshapeFunc(AlteraTamanhoJanela);
  	glutKeyboardFunc(Teclado);
	glutSpecialFunc(tecladoSpec);
	glutMouseFunc(GerenciaMouse);
	Inicializa();
	glutMainLoop();
	
}


// Função callback chamada para fazer o desenho
void Desenha(){

	if (life>0){
	
		glLineWidth(1);
		glClear(GL_COLOR_BUFFER_BIT);
		// cor de desenho:
		glColor3f(0.5,0.5,0.5);	
		// linha de contorno da tela
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0,0.0,0.0);	
		glVertex2f(0.02, 0.0);
		glVertex2f(0.02, win-0.02);
		glVertex2f(win-0.02, win-0.02);
		glVertex2f(win-0.02, 0.02);
		glEnd();
		desenhaObjeto();
		desenhaLinhas();
		//Executa os comandos OpenGL para renderização.
		glFlush();
	}else{
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(0,0,0);
		// Posiciona o texto stroke usando transformações geométricas
		glPushMatrix();	
		glTranslatef(15,100,0);
		glScalef(0.2, 0.2, 0.2); // diminui o tamanho do fonte
		//glRotatef(15, 0,0,1); // rotaciona o texto
		glLineWidth(2); // define a espessura da linha
		DesenhaTextoStroke(GLUT_STROKE_ROMAN,"GAME OVER");
		glPopMatrix();
		

		glFlush();
	}
}

// Muitos estados OpenGL devem ser determinados somente uma vez e não a cada vez que o rendering é realizado. AS funções dentro desta são executas uma vez só
void Inicializa(){
	//Define a cor de fundo da janela de visualização
	int i,j;
	life = 3;
	//zerando o mapa
	for (i=0; i<200; i++)
		for (j=0; j<200; j++)
		MAP[i][j]=0;
		
	srand(time(NULL));
	for (i=0; i<9; i++){
		random[i]=rand() % 150;
		printf("%d ",random[i]);
	}
	
	rand2 = rand() % 180;
	
	glClearColor(a,b,c,d);
	xf=iniX;
	yf=iniY;
	
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,win,0,win);
	
	glFlush();
}

// Função callback chamada para gerenciar eventos de teclas
void Teclado (unsigned char key, int x, int y)
{
	if (key == 27)
	exit(1);
}
// Teclas especiais
void tecladoSpec (int key, int x, int y){
		
		switch(key){
			case GLUT_KEY_UP: 
				if (yf<=win-1)
				yf+=1; 
				break;
			case GLUT_KEY_DOWN:
				if (yf>0)
				yf-=1;
				break;
			case GLUT_KEY_LEFT:
				if (xf>0)
				xf-=1;
				break;
			case GLUT_KEY_RIGHT:
				if (xf<=win-1)
				xf+=1;
		}

		colisao();
		if (!venceu())
		glutPostRedisplay();
}

void desenhaObjeto(){
	int i=0,j=0;
	glColor3f(b,c,0);
	glBegin(GL_QUADS);
		glVertex2f(xf, yf);
		glVertex2f(xf, yf+quad);
		glVertex2f(xf+quad, yf+quad);
		glVertex2f(xf+quad, yf);
	glEnd();
	
	glColor3f(0,97,150);
	glBegin(GL_QUADS);
		glVertex2f(rand2, 185);
		glVertex2f(rand2, 195);
		glVertex2f(rand2+10, 195);
		glVertex2f(rand2+10, 185);
	glEnd();
	
	for(i=rand2; i<rand2+10; i++){
		for(j=185; j<195; j++){
			MAP[i][j]=2;
//			glPointSize(3);
//			glBegin(GL_POINTS);
//			glVertex2i(i,j);
//			glEnd();	
		}
	}
	
}

void desenhaLinhas(){
	
	glColor3f(b,d,c);
	int i=0,j=1,x=0;
	
	for (i=0; i<9; i++){
		glLineWidth(esp);
		glBegin(GL_LINES);
		glVertex2i(0, j*20);
		glVertex2i(random[i], j*20);
		glVertex2i(random[i]+15, j*20);
		glVertex2i(200, j*20);
		glEnd();
	
		for (x=0; x<random[i]; x++){
				MAP[x][j*20]=1;
		}
		for (x=random[i]+15; x<200; x++){
				MAP[x+1][j*20]=1;
		}
		j++;
	}
}

int venceu(){
	int i;
	for (i=0; i<quad; i++)
		if (MAP[xf][yf+i]==2 || MAP[xf+i][yf]==2 || MAP[xf+i][yf+quad]==2 || MAP[xf+quad][yf+i]==2){
			glClear(GL_COLOR_BUFFER_BIT);
			glColor3f(0,0,0);
			// Posiciona o texto stroke usando transformações geométricas
			glPushMatrix();	
			glTranslatef(15,100,0);
			glScalef(0.2, 0.2, 0.2); // diminui o tamanho do fonte
			//glRotatef(15, 0,0,1); // rotaciona o texto
			glLineWidth(2); // define a espessura da linha
			DesenhaTextoStroke(GLUT_STROKE_ROMAN,"  VITORIA!!");
			glPopMatrix();
			glFlush();		
			return 1;
		}
	
}

void colisao(){
	int i;
	for (i=0; i<quad; i++)
		if (MAP[xf][yf+i]==1 || MAP[xf+i][yf]==1 || MAP[xf+i][yf+quad]==1 || MAP[xf+quad][yf+i]==1){
			xf=iniX;
			yf=iniY;
			life--;		
		}
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	// Especifica as dimensões da Viewport
	glViewport(0, 0, w, h);
	view_w = w;
	view_h = h;
	
	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(10.0f,50.0f,0,0);
	
}

void GerenciaMouse(int button, int state, int x, int y)
{
    
    srand(time(NULL));
    if (button == GLUT_LEFT_BUTTON)  // se o botão é o esquerdo do mouse
         if (state == GLUT_DOWN) {  // estado pressionado DOWN(para baixo) 
            a = rand() % 101;
            a = a/100;

            b = rand() % 101;
            b = b/100;

            c = rand() % 101;
            c = c/100;

            d = rand() % 101;
            d = d/100;

            glColor3f(a, b, c);
            glClearColor(d,c,b,a);
         }
    glutPostRedisplay();
}

void DesenhaTextoStroke(void *font, char *string) 
{  
	// Exibe caractere a caractere
	while(*string)
		glutStrokeCharacter(GLUT_STROKE_ROMAN,*string++); 
}

void DesenhaTexto(void *font, char *string) 
{
	// Exibe caractere a caractere
	while(*string)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,*string++); 
}
