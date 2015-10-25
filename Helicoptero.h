#ifndef HELICOPTERO_H
#define HELICOPTERO_H

#include "Rectangle.h"
#include "Circle.h"
#include "Polyline.h"
#include "tinyxml2.h"
#include "Tiro.h"
#include "Time.h"
#include "ObjetoResgate.h"

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
protected:
	// Definindo velocidade do helicoptero
	float velocidadeHelicoptero;
	
	// Define se o helicoptero é do tipo "jogador" ou do tipo "inimigo"
	string tipo;
	
	// Verifica se o helicoptero foi atingido
	bool foiAtingido;
	
	// valor se tipo = jogador
	Time* tempo; 
	bool temCombustivel;
	
	// valores se tipo = inimigo
	float freqTiro;
	
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
	float escalaHelicoptero;
	float enableMovimento;
	float posX;
	float posY;
	float anguloGiro;
	
	// Tiro
	vector<Tiro>* tiros;
	
	// Mira
	float anguloMira;
	float posMiraAnteriorX;
	float posMiraAnteriorY;
	
	// Propriedades do corpo do helicoptero
	Circle* dadosCircle; // Define qual o círculo do helicoptero
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
	// Método responsável por definir qual o círculo do helicoptero
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
	
	// Movimentação
	void moverParaFrente(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	void moverParaTras(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	void mudarEscalaMovimento();
	
	// Rotação
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
	void verificaTirosJogador(vector<Helicoptero>* inimigos, float quantidadeInimigos);
	
	// combustível
	void atualizarCombustivel(Rectangle* postoAbastecimento);
	
	// resgatar Objeto Resgate
	void resgatarObjeto(vector<ObjetoResgate>* objetosResgate);
	
	// detecta colisooes
	bool detectarLimitesArena(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito);
	bool detectarLimitesHelicopteros(vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador);
	bool detectarColisaoHelicoptero(Circle* c);
	
	
	void desenharHelicoptero();
	
	void definirCor (float R, float G, float B);
	
	//Getters and Setters
	void setVelocidade (float v) {
		this->velocidadeHelicoptero = v;
	}
	float getVelocidade () {
		return this->velocidadeHelicoptero;
	}
	
	void setTempoDeVoo (float t) {
		cout << "Setando tempo de Voo\n";
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
};

#endif
