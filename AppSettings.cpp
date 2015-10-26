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
	this->dadosArena = new Rectangle();
	this->postoAbastecimento = new Rectangle();
	this->dadosJogador = new Circle();

	this->dadosInimigos = new vector<Circle>();
	this->quantidadeInimigos = 0;
	
	this->dadosObjetoResgate = new vector<Circle>();
	this->objetosResgate = new vector<ObjetoResgate>();
	this->quantidadeObjetosResgate = 0;
	
	// Finaliza��o do jogo
	this->inimigosAindaVivos = 0;
	this->objetosAindaDevemSerResgatados = 0;
	this->mostrarTelaDeJogo = true;

	this->velocidadeHelicoptero = 0.0;
	this->jogador = new Helicoptero();
	this->inimigos = new vector<Helicoptero>();
	
	this->velocidadeTiro = 0.0;
	this->tiro = new Tiro();
	
	this->tempoDeVoo = 0.0;
	velocidadeHelicopteroInimigo = 0.0;
	freqTiro = 0.0;
	
	this->combustivel = new Rectangle();
}

AppSettings::~AppSettings() {
	// TODO Auto-generated destructor stub
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
	XMLElement* jogador = aplicacao->FirstChildElement(
			"helicoptero");
	if (jogador == NULL) {
		printf(
				"Erro na hora de encontrar Element 'helicoptero'! Finalizando programa...\n");
		exit(1);
	}
	
	this->carregarInformacoesHelicoptero(jogador);
	this->carregarInformacoesTiro();
	
	// Carregar dados helicoptero inimigo
	XMLElement* inimigo = aplicacao->FirstChildElement(
			"helicopteroInimigo");
	if (jogador == NULL) {
		printf(
				"Erro na hora de encontrar Element 'helicopteroInimigo'! Finalizando programa...\n");
		exit(1);
	}
	
	this->carregarInformacoesHelicopteroInimigo(inimigo);
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
			this->dadosArena->setValues(rectElem);
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
	
	int identificadorContagem = 0;
	
	while (circleElem != NULL) {
		aux = (circleElem->Attribute("id"));
		id = aux;

		if (id == "Jogador") {
			this->dadosJogador->setValues(circleElem);
			this->checkCircles(this->dadosJogador);
		}

		if (id == "Inimigo") {
			Circle* enemyCircle = new Circle();
			enemyCircle->setValues(circleElem);
			this->checkCircles(enemyCircle);
			this->dadosInimigos->push_back(*enemyCircle);
			this->quantidadeInimigos++;
		}

		if (id == "ObjetoResgate") {
			Circle* rescueObjectCircle = new Circle();
			ObjetoResgate* obj = new ObjetoResgate();
			
			rescueObjectCircle->setValues(circleElem);
			this->checkCircles(rescueObjectCircle);
			this->dadosObjetoResgate->push_back(*rescueObjectCircle);
			
			// seta os dados do objeto resgate com os setados anteriormente
			obj->dadosObjetoResgate = rescueObjectCircle;
			// adiciona o objeto no vetor de objetos Resgate
			this->objetosResgate->push_back(*obj);
			
			this->quantidadeObjetosResgate++;
		}

		this->objetosAindaDevemSerResgatados = this->quantidadeObjetosResgate;
		this->inimigosAindaVivos = this->quantidadeInimigos;
		circleElem = circleElem->NextSiblingElement("circle");
	}

	// Condições de saída do programa
	if (this->quantidadeInimigos < 1 || this->quantidadeObjetosResgate < 1) {
		cout << "Não há a quantidade mínima de inimigos e/ou objetos de resgate! Finalizando programa...\n";
	}

}

void AppSettings::showValues() {
	cout << "Imprimindo AppSettings\n";
	this->dadosArena->showValues();
	this->postoAbastecimento->showValues();
	this->dadosJogador->showValues();

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
	Rectangle* dadosArena = this->getDadosArena();
	float limiteSuperior = dadosArena->getY();
	float limiteInferior = dadosArena->getY() + dadosArena->getHeight();
	float limiteEsquerdo = dadosArena->getX();
	float limiteDireito = dadosArena->getX() + dadosArena->getWidth();


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
	detectarCircle(x, y, this->getDadosJogador());
}

void AppSettings::detectarInimigo (float x, float y) {
	Circle aux;
	
	int i = 0;

	for (i = 0; i < this->quantidadeInimigos; i ++) {
		aux = this->dadosInimigos->at(i);
		detectarCircle(x, y, &aux);
	}
}

void AppSettings::detectarObjetoResgate (float x, float y) {
	Circle aux;
	int i = 0;
	
	for (i = 0; i < this->quantidadeObjetosResgate; i ++) {
		aux = this->dadosObjetoResgate->at(i);
		detectarCircle(x, y, &aux);
	}
}

void AppSettings::detectarCircle (float x, float y, Circle* c) {
	if (c->internoCircunferencia(x, y)) {
		cout << "id=" << c->getId() << "\n";
	}
}

void AppSettings::detectarArena (float x, float y) {
	if (this->dadosArena->detectarRectangle(x, y)) {
		cout << "id=" <<  this->dadosArena->getId() << "\n";
	}
}

void AppSettings::detectarPostoDeAbastecimento (float x, float y) {
	if (this->postoAbastecimento->detectarRectangle(x, y)) {
		cout << "id=" <<  this->postoAbastecimento->getId() << "\n";
	}
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
	desenharRectangle(this->getDadosArena(), 1.0, 1.0, 1.0);
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
	desenharCircle(this->dadosJogador, 0.0, 1.0, 0.0);
}

void AppSettings::desenharInimigos() {
	int j = 0;
	Circle it;
	
	for (j = 0; j < this->getQuantidadeInimigos(); j++) {
		it = this->getDadosInimigos()->at(j);
		it.desenharCircleSemTransformacao(1.0, 0.0, 0.0);
		//desenharCircle(&it, 1.0, 0.0, 0.0);
	}
}

void AppSettings::desenharObjetosResgate() {
	int j = 0;
	ObjetoResgate* it;
	
	for (j = 0; j < this->getQuantidadeObjetosResgate(); j++) {
		it = &(this->objetosResgate->at(j));
		
		if (it->objetoResgatado == false) {
			it->desenharObjetoResgate();
		}
	}
}

void AppSettings::desenharHelicoptero() {
	
//	this->helicoptero->setarValores(this->getJogador());
	this->jogador->desenharHelicoptero();
	this->jogador->mostrarTiros();
	
	Helicoptero* inimigo;
	int i = 0;
	for (i = 0; i < this->quantidadeInimigos; i++) {
		inimigo = &(this->inimigos->at(i));
		inimigo->desenharHelicoptero();
		inimigo->mostrarTiros();
	}
}

void AppSettings::imprimirVenceuJogo(){
	// Teste para imprimir algo na tela!
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(1, 0, 0);
	
	float posicaoTextoX = this->getDadosArena()->getHeight()/2;
	float posicaoTextoY = this->getDadosArena()->getWidth()/2;
	
	glRasterPos3f(posicaoTextoX, posicaoTextoY, 0.0);
	const char* msg = "VOCE GANHOU!!!";
	while (*msg) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *msg);
		msg++;
	}
}

void AppSettings::imprimirPerdeuJogo(){

	// Teste para imprimir algo na tela!
	glPushAttrib(GL_ENABLE_BIT);
	glColor3f(1, 0, 0);

	float posicaoTextoX = this->getDadosArena()->getHeight()/2;
	float posicaoTextoY = this->getDadosArena()->getWidth()/2;

	glRasterPos3f(posicaoTextoX, posicaoTextoY, 0.0);
	const char* msg = "VOCE PERDEU!!!";
	while (*msg) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *msg);
		msg++;
	}
}

void AppSettings::imprimirMensagem (bool venceu){
	if (venceu) {
		this->imprimirVenceuJogo();
	} else {
		this->imprimirPerdeuJogo();
	}
}

void AppSettings::desenharObjetos() {	
	if (this->mostrarTelaDeJogo) {
		this->desenharArena();
		this->desenharPostoAbastecimento();
		this->desenharCombustivel();
		this->desenharObjetosResgate();
		this->desenharHelicoptero();
	}

	if (this->objetosAindaDevemSerResgatados <= 0) {
		this->mostrarTelaDeJogo = false;
		this->imprimirMensagem(true);
	}
	
	if (this->jogador->getFoiAtingido() == true) {
		this->mostrarTelaDeJogo = false;
		this->imprimirMensagem(false);
	}
}

void AppSettings::setarPosicaoHelicopteros(){
	this->getJogador()->setarValores(this->getDadosJogador());
	Helicoptero* inimigo;

	int i = 0;
	for (i = 0; i < this->quantidadeInimigos; i++) {
		inimigo = &(this->inimigos->at(i));
		inimigo->setarValores(&this->getDadosInimigos()->at(i));
	}
}

void AppSettings::carregarHelicopteroJogador() {
	this->jogador->identificador = 0;
	this->jogador->setVelocidade(this->velocidadeHelicoptero);
	this->tiro->setVelocidade(this->velocidadeTiro);
	this->jogador->setTempoDeVoo(this->tempoDeVoo);
	this->jogador->definirCor(0.0, 1.0, 0.0);
	this->jogador->carregarInformacoes();
	this->jogador->setTipo("jogador");
}

void AppSettings::carregarHelicopteroInimigos() {
	int i = 0;
	
	int contador = 1;
	
	for (i = 0; i < this->quantidadeInimigos; i++) {
		Helicoptero* inimigo = new Helicoptero();
		
		// identificador para identificar helicoptero
		inimigo->identificador = contador;
		contador++;
		
		inimigo->setVelocidade(this->velocidadeHelicopteroInimigo);
		inimigo->setFreqTiro(this->freqTiro);
		
		inimigo->definirCor(1.0, 0.0, 0.0);
		inimigo->mudarEscalaMovimento();
		inimigo->carregarInformacoes();
		
		inimigo->anguloGiro = rand() % 360;
		
		inimigo->setTipo("inimigo");
		this->inimigos->push_back(*inimigo);		
	}
}

void AppSettings::carregarInformacoesHelicopteros() {
	this->carregarHelicopteroJogador();
	this->carregarHelicopteroInimigos();
	this->setarPosicaoHelicopteros();
}

// Carregar informa\E7\F5es do config.xml do helicoptero do jogador
void AppSettings::carregarInformacoesHelicoptero(XMLElement* elem) {
	elem->QueryFloatAttribute("velHelicoptero",  &this->velocidadeHelicoptero);
	elem->QueryFloatAttribute("velTiro", &this->velocidadeTiro);	
	elem->QueryFloatAttribute("tempoDeVoo", &this->tempoDeVoo);	
}

// Carregar informa\E7\F5es do config.xml do helicoptero inimigo
void AppSettings::carregarInformacoesHelicopteroInimigo(XMLElement* elem) {
	elem->QueryFloatAttribute("freqTiro", &this->freqTiro);	
	elem->QueryFloatAttribute("velHelicoptero",  &this->velocidadeHelicopteroInimigo);
}

void AppSettings::verificaTiros() {
	// verifica tiros jogador
	int quantidadeInimigosAtingidos = 0.0;
	quantidadeInimigosAtingidos = this->jogador->verificaTirosJogador(this->inimigos, this->quantidadeInimigos);
	this->inimigosAindaVivos -= quantidadeInimigosAtingidos;

	// verifica tiros inimigos
	int i = 0;
	Helicoptero* inimigo;
	bool jogadorAtingido = false;
	
	for (i = 0; i < this->inimigosAindaVivos; i ++) {
		inimigo = &(this->inimigos->at(i));
		if (!inimigo->foiAtingido) {
			jogadorAtingido = inimigo->verificaTiroInimigo(this->jogador);
			
			if (jogadorAtingido) {
				this->jogador->foiAtingido;
				return;
			}
		}
	}
}

void AppSettings::carregarDadosCombustivel() {
	// set Id
	this->combustivel->setId("combustivel");

	// set X
	float x = this->dadosArena->getX() + 20;
	this->combustivel->setX(x);

	// set Y
	float y = this->dadosArena->getY() + this->dadosArena->getHeight() - 60;
	this->combustivel->setY(y);

	// set Widht
	this->combustivel->setWidth(60);

	// set Height
	this->combustivel->setHeight(30);

	// set Fill
	this->combustivel->setFill("");

	// set stroke_width
	this->combustivel->setStrokeWidth(0);

	// set stroke
	this->combustivel->setStroke("");
}

void AppSettings::desenharCombustivel() {
	float xTranslated;
	float yTranslated;
	float R = 0.0, G = 1.0, B = 0.0;
	
	// desenhar bateria
	glPushMatrix(); // Mover-se para metade da bateria
		// verde -> 50% - 100%
		// amarelho -> 10% - 50%
		// vermelho -> 0% - 10%
		float tempoAtual = (float) this->getJogador()->getTempo()->tempoAtual;
		float tempoMaximo = (float) this->getJogador()->getTempo()->tempoMaximo;
		
		float proporcao = tempoAtual/tempoMaximo;
		
		if (proporcao > 0.5 && proporcao <= 1.0) {
			R = 0;
			G = 1;
			B = 0;
			
		} else if (proporcao > 0.1 && proporcao <= 0.5) {
			R = 1;
			G = 1;
			B = 0;
		} else if (proporcao > 0 &&proporcao <= 0.1) {
			R = 1;
			G = 0;
			B = 0;
		} else if (proporcao < 0) {
			glPopMatrix();
		}
		float w = this->combustivel->getWidth()*proporcao;
		
		xTranslated = (this->combustivel->getX() + this->combustivel->getWidth()*proporcao)/2;
		yTranslated = this->combustivel->getY();
		
		glTranslated(xTranslated, yTranslated, 0.0);		
		this->combustivel->desenharRectangle(this->combustivel->getHeight(), w, R, G, B, false);
	glPopMatrix();
	
	// desenharContorno
	glPushMatrix();
		xTranslated = (this->combustivel->getX() + this->combustivel->getWidth())/2;
		yTranslated = this->combustivel->getY();
		glTranslated(xTranslated, yTranslated, 0.0);
		this->combustivel->desenharContorno(0.0, 0.0, 0.0);
	glPopMatrix();
}

void AppSettings::movimentarHelicopterosInimigos(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito) {
	int i = 0;
	Helicoptero* inimigo;
	
	for (i = 0; i < this->quantidadeInimigos; i++) {
		inimigo = &(this->inimigos->at(i));
		
		if (inimigo->getFoiAtingido() == false) {
		inimigo->movimentoAleatorio(limiteSuperior, limiteInferior, limiteEsquerdo, limiteDireito, this->inimigos, this->jogador);
		// implementar funcionalidade de atirar aleatoriamente
		}
		
		inimigo->efetuarTiroInimigo(this->getTiro());
	}
}

// Getters and Setters
string AppSettings::getPathArena() {
	return this->pathArena;
}

void AppSettings::setPathArena(string pathArena) {
	this->pathArena = pathArena;
}

Rectangle* AppSettings::getDadosArena() {
	return this->dadosArena;
}

void AppSettings::setDadosArena(Rectangle* dadosArena) {
	this->dadosArena = dadosArena;
}

Rectangle* AppSettings::getPostoAbastecimento() {
	return this->postoAbastecimento;
}

void AppSettings::setPostoAbastecimento(Rectangle* postoAbastecimento) {
	this->postoAbastecimento = postoAbastecimento;
}

Circle* AppSettings::getDadosJogador() {
	return this->dadosJogador;
}

void AppSettings::setDadosJogador(Circle* dadosJogador) {
	this->dadosJogador = dadosJogador;
}

vector<Circle>* AppSettings::getDadosInimigos() {
	return this->dadosInimigos;
}

void AppSettings::setDadosInimigos(vector<Circle>* dadosInimigos) {
	this->dadosInimigos = dadosInimigos;
}

int AppSettings::getQuantidadeInimigos() {
	return this->quantidadeInimigos;
}

void AppSettings::setQuantidadeInimigos(int quantidadeInimigos) {
	this->quantidadeInimigos = quantidadeInimigos;
}

vector<Circle>* AppSettings::getDadosObjetoResgate() {
	return this->dadosObjetoResgate;
}

void AppSettings::setDadosObjetoResgate(vector<Circle>* dadosObjetoResgate) {
	this->dadosObjetoResgate = dadosObjetoResgate;
}

int AppSettings::getQuantidadeObjetosResgate() {
	return this->quantidadeObjetosResgate;
}

void AppSettings::setQuantidadeObjetosResgate(int quantidadeObjetoResgate) {
	this->quantidadeObjetosResgate = quantidadeObjetosResgate;
}
