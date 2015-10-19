/*
 * AppSettings.h
 *
 *  Created on: 13/09/2015
 *      Author: rodrigo
 */

#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <string>
#include <vector>

#include "Rectangle.h"
#include "Circle.h"
#include "Helicoptero.h"
#include "Tiro.h"

using namespace std;

class AppSettings {
protected:
	string pathArena;
	Rectangle* dadosArena;
	Rectangle* postoAbastecimento;
	Circle* dadosJogador;
	vector<Circle>* dadosInimigos;
	int quantidadeInimigos;

	vector<Circle>* objetoResgate;
	int quantidadeObjetosResgate;
	
	// variavel que representa o helicoptero do jogador
	Helicoptero* jogador;
	vector<Helicoptero>* inimigos;
	
	Tiro* tiro;
	
	float velocidadeHelicoptero;
	float velocidadeHelicopteroInimigo;
	float freqTiro;
	float velocidadeTiro;
	float tempoDeVoo;

private:
	void detectarLimitesArena(float x, float y, float width, float height);
	void checkCircles (Circle* circle);
	
	// Detectar objetos
	void detectarCircle (float x, float y, Circle* c);
	void detectarInimigo (float x, float y);
	void detectarJogador(float x, float y);
	void detectarObjetoResgate(float x, float y);
	void detectarArena(float x, float y);
	void detectarPostoDeAbastecimento(float x, float y);

	// Desenhar Objetos
	void desenharArena();
	void desenharPostoAbastecimento();
	void desenharObjetosResgate();
	void desenharJogador();
	void desenharInimigos();
	void desenharHelicoptero();

	
public:
	AppSettings();
	virtual ~AppSettings();
	
	void carregarInformacoesHelicoptero(XMLElement* elem);
	void carregarInformacoesHelicopteroInimigo(XMLElement* elem);
	
	void carregarInformacoesHelicoptero();
	void carregarHelicopteroJogador();
	void carregarHelicopteroInimigos();
	void setarPosicaoHelicopteros();
	
	void carregarInformacoesTiro();
	void loadConfigXML (char** path);
	void loadSvgFile ();
	void showValues ();
	void detectarObjetos(float x, float y);
	void desenharObjetos();

	// Getters and Setters
	string getPathArena();
	void setPathArena(string pathArena);
	
	Rectangle* getDadosArena();
	void setDadosArena(Rectangle* dadosArena);
	Rectangle* getPostoAbastecimento();
	void setPostoAbastecimento(Rectangle* postoAbastecimento);
	
	Circle* getDadosJogador();
	void setDadosJogador (Circle* dadosJogador);
	
	vector<Circle>* getDadosInimigos();
	void setDadosInimigos (vector<Circle>* dadosInimigos);
	int getQuantidadeInimigos();
	void setQuantidadeInimigos(int quantidadeInimigos);

	vector<Circle>* getObjetoResgate ();
	void setObjetoResgate (vector<Circle>* objetoResgate);
	int getQuantidadeObjetosResgate();
	void setQuantidadeObjetosResgate (int quantidadeObjetoResgate);
	
	Helicoptero* getJogador() {return this->jogador;}
	Tiro* getTiro() {return this->tiro;}
	
	vector<Helicoptero>* getInimigos() {
		return this->inimigos;
	}

};

#endif /* APPSETTINGS_H_ */
