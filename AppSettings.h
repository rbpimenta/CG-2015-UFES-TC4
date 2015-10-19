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
	Rectangle* arena;
	Rectangle* postoAbastecimento;
	Circle* jogador;
	vector<Circle>* inimigos;
	int quantidadeInimigos;

	vector<Circle>* objetoResgate;
	int quantidadeObjetosResgate;
	
	Helicoptero* helicoptero;
	
	Tiro* tiro;
	
	float velocidadeHelicoptero;
	float velocidadeTiro;

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
	void carregarInformacoesTiro();
	void loadConfigXML (char** path);
	void loadSvgFile ();
	void showValues ();
	void detectarObjetos(float x, float y);
	void desenharObjetos();

	// Getters and Setters
	string getPathArena();
	void setPathArena(string pathArena);
	Rectangle* getArena();
	void setArena(Rectangle* arena);
	Rectangle* getPostoAbastecimento();
	void setPostoAbastecimento(Rectangle* postoAbastecimento);
	Circle* getJogador();
	void setJogador (Circle* jogador);
	vector<Circle>* getInimigos();
	void setInimigos(vector<Circle>* inimigos);
	int getQuantidadeInimigos();
	void setQuantidadeInimigos(int quantidadeInimigos);

	vector<Circle>* getObjetoResgate ();
	void setObjetoResgate (vector<Circle>* objetoResgate);
	int getQuantidadeObjetosResgate();
	void setQuantidadeObjetosResgate (int quantidadeObjetoResgate);
	
	Helicoptero* getHelicoptero() {return this->helicoptero;}
	Tiro* getTiro() {return this->tiro;}

};

#endif /* APPSETTINGS_H_ */
