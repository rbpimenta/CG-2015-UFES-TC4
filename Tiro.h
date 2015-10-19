#ifndef TIRO_H
#define TIRO_H

#include "Circle.h"
#include "tinyxml2.h"

#include <iostream>

using namespace tinyxml2;
using namespace std;

class Tiro
{
	protected:
		float velocidadeTiro;
		Circle* tiro;
		
		// Informações para mostrar o tiro
		float fatorEscala;
		float fatorEscalaInverso;
		float cx; 
		float cy;
		float raio;
		float anguloGiro;
		float anguloMira;
		float miraHeight;
		float posX;
		float posY;
	
	public:
		Tiro();
		Tiro(float fatorEscala,  float fatorEscalaInverso, float cx, float cy, float raio, float anguloGiro, float anguloMira, float miraHeight, float posX, float posY);
		~Tiro();
		void carregarInformacoes();
		void desenharTiro();
		void movimentarParaFrente();
		void movimentarParaTras();
		void showValues();
		
		void setVelocidade (float v) { this->velocidadeTiro = v;}
		float getVelocidade () { return this->velocidadeTiro;}
		
		void setTiro(Tiro* t) {
			this->tiro->setCx(t->tiro->getCx());
			this->tiro->setCy(t->tiro->getCy());
			this->tiro->setR(t->tiro->getR());
			this->velocidadeTiro = t->getVelocidade();
		}

};

#endif
