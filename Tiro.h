#ifndef TIRO_H
#define TIRO_H

#include "Circle.h"
#include "tinyxml2.h"
#include "Transformacao.h"

#include <iostream>

using namespace tinyxml2;
using namespace std;

class Tiro
{
	public:
		
		Transformacao* transformacao;
		
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
		bool verificarLimites(float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito);
		void movimentarParaFrente();
		void movimentarParaTras();
		void showValues();
		
		// Getters and Setters
		void setVelocidade (float v) { this->velocidadeTiro = v;}
		float getVelocidade () { return this->velocidadeTiro;}
		
		void setTiro(Tiro* t) {
			this->tiro->setCx(t->tiro->getCx());
			this->tiro->setCy(t->tiro->getCy());
			this->tiro->setR(t->tiro->getR());
			this->velocidadeTiro = t->getVelocidade();
		}

		float getPosX() {
			return this->posX;
		}
		void setPosX(float x) {
			this->posX = x;
		}		

		float getPosY() {
			return this->posY;
		}
		void setPosY(float y) {
			this->posY = y;
		}	
			
};

#endif
