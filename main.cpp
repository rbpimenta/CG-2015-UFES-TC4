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

void keyPress(unsigned char key, int x, int y) {
	switch (key) {
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

void keyup(unsigned char key, int x, int y) {
	keyStatus[(int) (key)] = 0;
	glutPostRedisplay();
}

void idle(void) {
	// Detecta os limites da arena
	Rectangle* dadosArena = appSettings->getDadosArena();

	float limiteSuperior = dadosArena->getY();
	float limiteInferior = dadosArena->getY() + dadosArena->getHeight();
	float limiteEsquerdo = dadosArena->getX();
	float limiteDireito = dadosArena->getX() + dadosArena->getWidth();

	if (keyStatus['+'] == 1) {
		appSettings->getJogador()->aumentarGiroHelices();
	}

	if (keyStatus['-'] == 1) {

		appSettings->getJogador()->diminuirGiroHelices();
	}

	if (keyStatus['w'] == 1) {
		// so movimenta o helicoptero se o mesmo estiver dentro da arena
		appSettings->getJogador()->moverParaFrente(limiteSuperior,
				limiteInferior, limiteEsquerdo, limiteDireito,
				appSettings->getInimigos(), appSettings->getJogador());
	}

	if (keyStatus['s'] == 1) {
		// so movimenta o helicoptero se o mesmo estiver dentro da arena
		appSettings->getJogador()->moverParaTras(limiteSuperior, limiteInferior,
				limiteEsquerdo, limiteDireito, appSettings->getInimigos(),
				appSettings->getJogador());
	}

	if (keyStatus['a'] == 1) {
		appSettings->getJogador()->rotacionarEsquerda();
	}

	if (keyStatus['d'] == 1) {
		appSettings->getJogador()->rotacionarDireita();
	}

	appSettings->getJogador()->moverHelice();
	appSettings->getJogador()->movimentarTiros(limiteSuperior, limiteInferior,
			limiteEsquerdo, limiteDireito);

	int i = 0;
	Helicoptero* inimigo;
	for (i = 0; i < appSettings->getQuantidadeInimigos(); i++) {
		inimigo = &(appSettings->getInimigos()->at(i));
		inimigo->moverHelice();
		inimigo->movimentarTiros(limiteSuperior, limiteInferior, limiteEsquerdo,
				limiteDireito);
	}

	// Verifica os tiros dos helicopteros
	appSettings->verificaTiros();

	// Atualiza o combustível do jogador
	appSettings->getJogador()->atualizarCombustivel(
			appSettings->getPostoAbastecimento());

	// Verificando quantos objetos foram resgatados
	int objetosResgatados = 0;
	objetosResgatados = appSettings->getJogador()->resgatarObjeto(
			appSettings->getObjetosResgate());

	// Definindo quantos objetos de resgate ainda faltam
	appSettings->objetosAindaDevemSerResgatados -= objetosResgatados;
	objetosResgatados = 0;

	// movimentar helicopteros inimigos
	appSettings->movimentarHelicopterosInimigos(limiteSuperior, limiteInferior, limiteEsquerdo, limiteDireito);
	

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
			appSettings->getJogador()->mudarEscalaMovimento();
		}
		if (button == GLUT_LEFT_BUTTON) {
			appSettings->getJogador()->realizarTiro(appSettings->getTiro());
		}
	}
}

void mouseMove(int x, int y) {
	appSettings->getJogador()->rotacionarMira(x, y);
}

int main(int argc, char** argv) {
	appSettings->loadConfigXML(argv);
	appSettings->loadSvgFile();

	appSettings->carregarDadosCombustivel();
	appSettings->carregarInformacoesHelicopteros();

	// Iniciando tela e demais variáveis
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	// Setando variáveis da tela
	DISPLAY_WIDTH = appSettings->getDadosArena()->getWidth();
	DISPLAY_HEIGHT = appSettings->getDadosArena()->getHeight();

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
