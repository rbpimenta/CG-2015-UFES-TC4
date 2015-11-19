#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include "Rectangle.h"
#include "Circle.h"
#include "Polyline.h"
#include "tinyxml2.h"
#include "Tiro.h"
#include "Time.h"
#include "ObjetoResgate.h"
#include "Transformacao.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include <string>
#include <math.h>

using namespace std;
using namespace tinyxml2;


class Helicoptero
{
public:
	int identificador;
	
	Transformacao* transformacao;
	
public:
	// Definindo velocidade do helicoptero
	float velocidadeHelicoptero;
	
	// Define se o helicoptero � do tipo "jogador" ou do tipo "inimigo"
	string tipo;
	
	// Verifica se o helicoptero foi atingido
	bool foiAtingido;
	
	// valor se tipo = jogador
	Time* tempo; 
	bool temCombustivel;
	
	// valores se tipo = inimigo
	float freqTiro;
	float anguloMovimentoAleatorio;
	bool movimentoAleatorioRotacao;
	float tempoUltimoDisparo;
	
	// Helice
	float velocidadeHelices;
	float anguloHelices;
	float aceleracaoHelices;
	
	// Transformacao
	float fatorEscala;
	float cx;
	float cy;
	float raio;
	
	// Movimentacao
	float anguloGiro;
	float escalaHelicoptero;
	float enableMovimento;
	float posX;
	float posY;
	
	bool noLimiteDaArena;

	// Tiro
	vector<Tiro>* tiros;
	
	// Mira
	float anguloMira;
	float posMiraAnteriorX;
	float posMiraAnteriorY;
	
	// Propriedades do corpo do helicoptero
	Circle* dadosCircle; // Define qual o c�rculo do helicoptero
	Rectangle* mira;
	Rectangle* corpo;
	Rectangle* cauda;
	Rectangle* caudaEsquerda;
	Rectangle* caudaDireita;	
	Circle* centroHelice;
	vector<Polyline>* helices;
	
public:
	Helicoptero();
	~Helicoptero();
	void carregarInformacoes();
	// M�todo respons�vel por definir qual o c�rculo do helicoptero
	void setarValores(Circle* c);
	void showValues();
	
	// Tiro
	void realizarTiro(Tiro* t);
	void mostrarTiros();
	void movimentarTiros(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito);
	
	// Mira
	void rotacionarMira (float x, float y);
	void rotacionarMiraDireita();
	void rotacionarMiraEsquerda();
	
	// Helices
	void aumentarGiroHelices();
	void diminuirGiroHelices();
	void moverHelice();
	
	// Movimenta��o
	void moverParaFrente(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	void moverParaTras(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	void mudarEscalaMovimento();
	
	// Rota��o
	void rotacionarDireita();
	void rotacionarEsquerda();
	
	// Desenhar Helicoptero
	float color[3];
	void desenharMira();
	void desenharCorpo();
	void desenharCaudaPrincipal();
	void desenharCaudaEsquerda();
	void desenharCaudaDireita();
	void desenharCentroHelice();
	void desenharHelices(Circle* c);
	
	// tiros
	int verificaTirosJogador(vector<Helicoptero>* inimigos, float quantidadeInimigos);
	bool verificaTiroInimigo(Helicoptero* helicopteroJogador);
	
	// combust�vel
	void atualizarCombustivel(Rectangle* postoAbastecimento);
	
	// resgatar Objeto Resgate
	int resgatarObjeto(vector<ObjetoResgate>* objetosResgate);
	
	// detecta colisooes
	bool detectarLimitesArena(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito);
	bool detectarLimitesHelicopteros(vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	bool detectarColisaoHelicoptero(Circle* c);
	
	void desenharHelicoptero();
	
	void definirCor (float R, float G, float B);
	
	void movimentoAleatorio(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	void efetuarTiroInimigo(Tiro* t);

	//Getters and Setters
	void setVelocidade (float v) {
		this->velocidadeHelicoptero = v;
	}
	float getVelocidade () {
		return this->velocidadeHelicoptero;
	}
	
	void setTempoDeVoo (float t) {
		this->tempo->tempoMaximo = (int) t;
		this->tempo->tempoAtual = (int)  t;
		this->tempo->tempoUltimaCarga = 0.0;
	}
	
	void setFreqTiro (float f) {
		this->freqTiro = f;
	}
	float getFreqTiro () {
		return this->freqTiro;
	}
	
	void setTipo (string t) {
		this->tipo = t;
	}
	string getTipo () {
		return this->tipo;
	}
	
	Time* getTempo() {
		return this->tempo;
	}
	
	void setDadosCircle (Circle* c) {
		this->dadosCircle = c;
	}
	
	bool getFoiAtingido() {
		return this->foiAtingido;
	}
};

#endif
