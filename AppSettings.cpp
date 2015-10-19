/*
 * AppSettings.cpp
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#include "AppSettings.h"
#include "tinyxml2.h"
#include "Rectangle.h"
#include "Circle.h"

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <math.h>

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;
using namespace tinyxml2;

AppSettings::AppSettings() {
	// TODO Auto-generated constructor stub
	this->pathArena = "";
	this->arena = new Rectangle();
	this->postoAbastecimento = new Rectangle();
	this->jogador = new Circle();

	this->inimigos = new vector<Circle>();
	this->quantidadeInimigos = 0;
	
	this->objetoResgate = new vector<Circle>();
	this->quantidadeObjetosResgate = 0;
	
	this->velocidadeHelicoptero = 0.0;
	this->helicoptero = new Helicoptero();
	
	this->velocidadeTiro = 0.0;
	this->tiro = new Tiro();
	
}

AppSettings::~AppSettings() {
	// TODO Auto-generated destructor stub
}

void AppSettings::carregarInformacoesHelicoptero(XMLElement* elem) {
	elem->QueryFloatAttribute("velHelicoptero",  &this->velocidadeHelicoptero);
	this->helicoptero->setVelocidade(this->velocidadeHelicoptero);
	
	elem->QueryFloatAttribute("velTiro", &this->velocidadeTiro);
	this->tiro->setVelocidade(this->velocidadeTiro);
	
	this->helicoptero->carregarInformacoes();
}

void AppSettings::carregarInformacoesTiro() {
	this->tiro->carregarInformacoes();
}

/*
 * irá carregar o nome do arquivo SVG
 */
void AppSettings::loadConfigXML(char** path) {
	// Carregar arquivo XML principal
	XMLDocument* doc = new XMLDocument();

	// Buscando arquivo de configuração
	string strPath = path[1];
	strPath = strPath + "config.xml";

	const char* pFilename = NULL;
	pFilename = strPath.data();

	if (doc->LoadFile(pFilename) == XML_ERROR_FILE_NOT_FOUND) {
		printf("Erro na hora de abrir arquivo! Finalizando programa...\n");
		exit(1);
	}

	XMLElement* aplicacao = doc->FirstChildElement("aplicacao");
	if (aplicacao == NULL) {
		printf(
				"Erro na hora de encontrar Element 'aplicacao'! Finalizando programa...\n");
		exit(1);
	}

	XMLElement* arquivoEntrada = aplicacao->FirstChildElement(
			"arquivosDeEntrada");
	if (arquivoEntrada == NULL) {
		printf(
				"Erro na hora de encontrar Element 'arquivoEntrada'! Finalizando programa...\n");
		exit(1);
	}

	// Definindo o nome do arquivo da arena
	XMLElement* arquivoDaArena = arquivoEntrada->FirstChildElement(
			"arquivoDaArena");

	const char* nome = "";
	const char* tipo = "";
	const char* caminho = "";

	nome = arquivoDaArena->Attribute("nome");
	tipo = arquivoDaArena->Attribute("tipo");
	caminho = arquivoDaArena->Attribute("caminho");

	string nomeSvg(nome);
	string tipoSvg(tipo);
	string caminhoSvg(caminho);
	string total = caminhoSvg + nomeSvg + "." + tipoSvg;

	this->pathArena = total;
	
	// Carregar dados do helicoptero
	XMLElement* helicoptero = aplicacao->FirstChildElement(
			"helicoptero");
	if (helicoptero == NULL) {
		printf(
				"Erro na hora de encontrar Element 'helicoptero'! Finalizando programa...\n");
		exit(1);
	}
	
	this->carregarInformacoesHelicoptero(helicoptero);
	this->carregarInformacoesTiro();
	doc->~XMLDocument();
}

void AppSettings::loadSvgFile() {
	XMLDocument* doc = new XMLDocument();

	doc->LoadFile(this->pathArena.data());

	if (doc == NULL) {
		cout << "Problema ao abrir arquivo .svg\n";
		exit(1);
	}

	XMLElement* svg = doc->FirstChildElement("svg");
	if (svg == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'svg'! Finalizando programa...\n";
		exit(1);
	}

	XMLElement* rectElem = svg->FirstChildElement("rect");

	if (rectElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'rect'! Finalizando programa...\n";
		exit(1);
	}

	// atributos do tipo rect
	string id = ""; // Identificador
	const char* aux = "";

	while (rectElem != NULL) {
		// set Id
		aux = (rectElem->Attribute("id"));
		id = aux;

		// Carregar arena
		if (id == "Arena") {
			this->arena->setValues(rectElem);
		}

		// Carregar Posto de abastecimento
		if (id == "PostoAbastecimento") {
			this->postoAbastecimento->setValues(rectElem);
			this->detectarLimitesArena(this->postoAbastecimento->getX(),
					this->postoAbastecimento->getY(),
					this->postoAbastecimento->getWidth(),
					this->postoAbastecimento->getHeight());

		}

		rectElem = rectElem->NextSiblingElement("rect");
	}

	// Setando valores circle
	XMLElement* circleElem = svg->FirstChildElement("circle");
	if (circleElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'circle'! Finalizando programa...\n";
		exit(1);
	}

	while (circleElem != NULL) {
		aux = (circleElem->Attribute("id"));
		id = aux;

		if (id == "Jogador") {
			this->jogador->setValues(circleElem);
			this->checkCircles(this->jogador);
		}

		if (id == "Inimigo") {
			Circle* enemyCircle = new Circle();
			enemyCircle->setValues(circleElem);
			this->checkCircles(enemyCircle);
			this->inimigos->push_back(*enemyCircle);
			this->quantidadeInimigos++;
		}

		if (id == "ObjetoResgate") {
			Circle* rescueObjectCircle = new Circle();
			rescueObjectCircle->setValues(circleElem);
			this->checkCircles(rescueObjectCircle);
			this->objetoResgate->push_back(*rescueObjectCircle);
			this->quantidadeObjetosResgate++;
		}

		circleElem = circleElem->NextSiblingElement("circle");
	}

	// Condições de saída do programa
	if (this->quantidadeInimigos < 1 || this->quantidadeObjetosResgate < 1) {
		cout << "Não há a quantidade mínima de inimigos e/ou objetos de resgate! Finalizando programa...\n";
	}

}

void AppSettings::showValues() {
	cout << "Imprimindo AppSettings\n";
	this->arena->showValues();
	this->postoAbastecimento->showValues();
	this->jogador->showValues();

	int i = 0;

	Circle* it = NULL;
	for (i = 0; i < this->quantidadeInimigos; i ++) {
		it->showValues();
	}

	for (i = 0; i < this->quantidadeObjetosResgate; i ++) {
		it->showValues();
	}
	
	this->tiro->showValues();
}

void AppSettings::checkCircles (Circle* circle) {
	float x = circle->getCx();
	float y = circle->getCy();
	float raio = circle->getR();

	// Checa centor do círculo
	this->detectarLimitesArena (x, y, 0.0, 0.0);

	// Checa parte inferior do circulo
	this->detectarLimitesArena (x, y + raio, 0.0, 0.0);

	// Checa parte superior do circulo
	this->detectarLimitesArena (x, y - raio, 0.0, 0.0);

	// Checa parte mais à direita do círculo
	this->detectarLimitesArena (x + raio, y, 0.0, 0.0);

	// Checa parte mais à esquerda do círculo
	this->detectarLimitesArena (x - raio, y, 0.0, 0.0);
}

void AppSettings::detectarLimitesArena(float x, float y, float width, float height) {
	Rectangle* arena = this->getArena();
	float limiteSuperior = arena->getY();
	float limiteInferior = arena->getY() + arena->getHeight();
	float limiteEsquerdo = arena->getX();
	float limiteDireito = arena->getX() + arena->getWidth();


	if (y < limiteSuperior || y > limiteInferior
		|| (y + height) > limiteInferior) {
		cout << "Objeto fora dos limites da arena. Finalizando o programa...";
		exit(1);
	}

	if (x < limiteEsquerdo || x > limiteDireito
		|| (x + width) > limiteDireito) {
		cout << "Objeto fora dos limites da arena. Finalizando o programa...";
		exit(1);
	}

}

void AppSettings::detectarJogador (float x, float y) {
	detectarCircle(x, y, this->getJogador());
}

void AppSettings::detectarInimigo (float x, float y) {
	Circle aux;
	
	int i = 0;

	for (i = 0; i < this->quantidadeInimigos; i ++) {
		aux = this->inimigos->at(i);
		detectarCircle(x, y, &aux);
	}
}

void AppSettings::detectarObjetoResgate (float x, float y) {
	Circle aux;
	int i = 0;
	
	for (i = 0; i < this->quantidadeObjetosResgate; i ++) {
		aux = this->objetoResgate->at(i);
		detectarCircle(x, y, &aux);
	}
}

bool internoCircunferencia(float x, float y, Circle* circle) {

	float distX = pow ((circle->getCx() - x), 2.0);
	float distY = pow ((circle->getCy() - y), 2.0);
	float rPow2 = pow (circle->getR(), 2.0);
	float soma = distX + distY;

	if (soma <= rPow2) {
		return true;
	}

	return false;
}

void AppSettings::detectarCircle (float x, float y, Circle* c) {
	if (internoCircunferencia(x, y, c)) {
		cout << "id=" << c->getId() << "\n";
	}
}

void detectarRectangle (Rectangle* r, float x, float y) {
	if (x > r->getX()
		&& x < (r->getX() + r->getWidth())) {

		if (y > r->getY()
			&& y < (r->getY() + r->getHeight())) {

			cout << "id=" <<  r->getId() << "\n";
		}
	}

}

void AppSettings::detectarArena (float x, float y) {
	detectarRectangle(this->arena, x, y);
}

void AppSettings::detectarPostoDeAbastecimento (float x, float y) {
	detectarRectangle(this->postoAbastecimento, x, y);
}

void AppSettings::detectarObjetos(float x, float y) {
	//detectarArena (x, y);
	detectarPostoDeAbastecimento(x, y);
	detectarJogador(x, y);
	detectarInimigo(x, y);
	detectarObjetoResgate(x, y);
}

void desenharRectangle(Rectangle* r, float R, float G, float B) {
		glColor3f(R, G, B);
		glBegin(GL_POLYGON);
		glVertex3f(r->getX(),
					r->getY(),
					0.0);
		glVertex3f(r->getX() + r->getWidth(),
					r->getY(),
					0.0);
		glVertex3f(r->getX()  + r->getWidth(),
					r->getY() + r->getHeight(),
					0.0);
		glVertex3f(r->getX(),
					r->getY() + r->getHeight(),
					0.0);
	glEnd();
}

void AppSettings::desenharArena() {
	desenharRectangle(this->getArena(), 1.0, 1.0, 1.0);
}

void AppSettings::desenharPostoAbastecimento() {
	desenharRectangle(this->getPostoAbastecimento(), 0.75, 0.75, 0.75);
}

void desenharCircle (Circle* c, float R, float G, float B) {
	int i = 0;
	int num_segments = 100;
	float twicePi = 2*M_PI;
	float radius = c->getR();
		
	glColor3f(R, G, B);
		// Desenhar a parte interna do círculo
		glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
		glVertex3f(c->getCx(), c->getCy(), 0.0); // center of circle
		   for (i = 0; i <= 20; i++)   {
				glVertex3f (
					(c->getCx() + (radius * cosf(i * twicePi / 20))),
					(c->getCy() + (radius * sin(i * twicePi / 20))),
					0.0);
			}
		glEnd();

		// Desenhar a parte da superfície do círculo
		glBegin(GL_LINE_LOOP);
			for (i = 0; i < num_segments; i++)   {
				float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
				float x = c->getR() * cosf(theta);//calculate the x component
				float y = c->getR() * sinf(theta);//calculate the y component
				glVertex3f(x + c->getCx(),
							y + c->getCy(),
							0.0);//output vertex
			}
		glEnd();
	
}

void AppSettings::desenharJogador() {
	desenharCircle(this->jogador, 0.0, 1.0, 0.0);
}

void AppSettings::desenharInimigos() {
	int j = 0;
	Circle it;
	
	for (j = 0; j < this->getQuantidadeInimigos(); j++) {
		it = this->getInimigos()->at(j);
		desenharCircle(&it, 1.0, 0.0, 0.0);
	}
}

void AppSettings::desenharObjetosResgate() {
	int j = 0;
	Circle it;
	
	for (j = 0; j < this->getQuantidadeObjetosResgate(); j++) {
		it = this->getObjetoResgate()->at(j);
		desenharCircle(&it, 0.0, 0.0, 1.0);
	}
}

void AppSettings::desenharHelicoptero() {
	
//	this->helicoptero->setarValores(this->getJogador());
	this->helicoptero->desenharHelicoptero();
	this->helicoptero->mostrarTiros();
}

void AppSettings::desenharObjetos() {	
	this->desenharArena();
	this->desenharPostoAbastecimento();
	this->desenharJogador();
	this->desenharInimigos();
	this->desenharObjetosResgate();
	this->desenharHelicoptero();
}

// Getters and Setters
string AppSettings::getPathArena() {
	return this->pathArena;
}

void AppSettings::setPathArena(string pathArena) {
	this->pathArena = pathArena;
}

Rectangle* AppSettings::getArena() {
	return this->arena;
}

void AppSettings::setArena(Rectangle* arena) {
	this->arena = arena;
}

Rectangle* AppSettings::getPostoAbastecimento() {
	return this->postoAbastecimento;
}

void AppSettings::setPostoAbastecimento(Rectangle* postoAbastecimento) {
	this->postoAbastecimento = postoAbastecimento;
}

Circle* AppSettings::getJogador() {
	return this->jogador;
}

void AppSettings::setJogador(Circle* jogador) {
	this->jogador = jogador;
}

vector<Circle>* AppSettings::getInimigos() {
	return this->inimigos;
}

void AppSettings::setInimigos(vector<Circle>* inimigos) {
	this->inimigos = inimigos;
}

int AppSettings::getQuantidadeInimigos() {
	return this->quantidadeInimigos;
}

void AppSettings::setQuantidadeInimigos(int quantidadeInimigos) {
	this->quantidadeInimigos = quantidadeInimigos;
}

vector<Circle>* AppSettings::getObjetoResgate() {
	return this->objetoResgate;
}

void AppSettings::setObjetoResgate(vector<Circle>* objetoResgate) {
	this->objetoResgate = objetoResgate;
}

int AppSettings::getQuantidadeObjetosResgate() {
	return this->quantidadeObjetosResgate;
}

void AppSettings::setQuantidadeObjetosResgate(int quantidadeObjetoResgate) {
	this->quantidadeObjetosResgate = quantidadeObjetosResgate;
}
