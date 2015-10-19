/*
 * main.cpp
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#include "tinyxml2.h"
#include "AppSettings.h"
#include "Rectangle.h"
#include "Circle.h"

#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <ctime>
#include <math.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>


// Variáveis referentes ao tamanho da tela
float DISPLAY_WIDTH = 0;
float DISPLAY_HEIGHT = 0;

AppSettings* appSettings = new AppSettings();

// Vértices
float globalX = 0.0, globalY = 0.0;

int keyStatus[256];

using namespace std;
using namespace tinyxml2;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	appSettings->desenharObjetos();
	glutSwapBuffers();
}

void keyPress(unsigned char key, int x, int y)
{
	switch (key)
    {
        case '+':
        	keyStatus['+'] = 1;
        	break;
        case '-':
        	keyStatus['-'] = 1;
        	break;
        case 'w':
        	keyStatus['w'] = 1;
        	break;
        case 's':
        	keyStatus['s'] = 1;
        	break;
        case 'a':
        	keyStatus['a'] = 1;
        	break;
        case 'd':
        	keyStatus['d'] = 1;
        	break;        	

    }
    glutPostRedisplay();
}

void keyup(unsigned char key, int x, int y)
{
    keyStatus[(int)(key)] = 0;
    glutPostRedisplay();
}

void idle(void) {
	
	// Detecta os limites da arena
	Rectangle* arena = appSettings->getArena();
	
	float limiteSuperior = arena->getY();
	float limiteInferior = arena->getY() + arena->getHeight();
	float limiteEsquerdo = arena->getX();
	float limiteDireito = arena->getX() + arena->getWidth();

	if(keyStatus['+'] == 1)
    {
        appSettings->getHelicoptero()->aumentarGiroHelices();
    }
    
    if(keyStatus['-'] == 1)
    {
    	
        appSettings->getHelicoptero()->diminuirGiroHelices();
    }
    
    if(keyStatus['w'] == 1)
    {
    	// so movimenta o helicoptero se o mesmo estiver dentro da arena  	
        appSettings->getHelicoptero()->moverParaFrente(limiteSuperior, limiteInferior, limiteEsquerdo, limiteDireito);        
    }
    
    if(keyStatus['s'] == 1)
    {
    	// so movimenta o helicoptero se o mesmo estiver dentro da arena  	
        appSettings->getHelicoptero()->moverParaTras(limiteSuperior, limiteInferior, limiteEsquerdo, limiteDireito);
    }

    if(keyStatus['a'] == 1)
    {
        appSettings->getHelicoptero()->rotacionarEsquerda();
    }

    if(keyStatus['d'] == 1)
    {
        appSettings->getHelicoptero()->rotacionarDireita();
    }
    
    appSettings->getHelicoptero()->moverHelice();
    appSettings->getHelicoptero()->movimentarTiros();
    glutPostRedisplay();
}

void init(void) {
	// Selecionar cor de fundo (preto)
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Inicializar sistema de viz
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0.0, 0.0, 1.0);
}

void mouse(int button, int state, int x, int y) {

	if (state == GLUT_DOWN) {
		globalX = (float) x;
		globalY = (float) y;
		
		if (button == GLUT_RIGHT_BUTTON) {
			appSettings->getHelicoptero()->mudarEscalaMovimento();
		}
		if (button == GLUT_LEFT_BUTTON) {
			appSettings->getHelicoptero()->realizarTiro(appSettings->getTiro());
		}
	}
}

void mouseMove (int x, int y) {
	appSettings->getHelicoptero()->rotacionarMira(x, y);
}

int main(int argc, char** argv) {
	appSettings->loadConfigXML(argv);
	appSettings->loadSvgFile();
	appSettings->getHelicoptero()->setarValores(appSettings->getJogador());

	// Iniciando tela e demais variáveis
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Setando variáveis da tela
	DISPLAY_WIDTH = appSettings->getArena()->getWidth();
	DISPLAY_HEIGHT = appSettings->getArena()->getHeight();

	glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("Trabalho 3");

	// Inicializa a janela
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);

    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyup);

	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMove);

	glutMainLoop();

	return 0;
}
