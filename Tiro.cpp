#include "Tiro.h"

Tiro::Tiro()
{
	this->velocidadeTiro = 0.0;
	this->tiro = new Circle();
	
	this->fatorEscala = 0.0;
	this->fatorEscalaInverso = 0.0;
	this->cx = 0.0;
	this->cy = 0.0;
	this->raio = 0.0;
	this->anguloGiro = 0.0;
	this->anguloMira = 0.0;
	this->posX = 0.0;
	this->posY = 0.0;
	this->anguloGiro = 0.0;
	
}

Tiro::Tiro(float fatorEscala,  float fatorEscalaInverso, float cx, float cy, float raio, float anguloGiro, float anguloMira, float miraHeight, float posX,	float posY) {
	this->velocidadeTiro = 0.0;
	this->tiro = new Circle();
	
	this->fatorEscala = fatorEscala;
	this->fatorEscalaInverso = fatorEscalaInverso;
	this->cx = cx;
	this->cy = cy;
	this->raio = raio;
	this->anguloGiro = anguloGiro;
	this->anguloMira = anguloMira;
	this->posX = posX;
	this->posY = posY;
	this->miraHeight = miraHeight;
	
}

Tiro::~Tiro()
{
}

void Tiro::carregarInformacoes() {
	XMLDocument* doc = new XMLDocument();

	doc->LoadFile("tiro.svg");

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
	
	XMLElement* circleElem = svg->FirstChildElement("circle");
	if (circleElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'circle'! Finalizando programa...\n";
		exit(1);
	}
	
	this->tiro->setValues(circleElem);
}

void Tiro::desenharTiro() {
	glPushMatrix(); // Escala
		// diminuir tamanho do helicoptero de acordo com tamanho do círculo
		glScalef(this->fatorEscala, this->fatorEscala, 0.0);
	
		float xTranslated = this->posX*this->fatorEscalaInverso;
		float yTranslated = this->posY*this->fatorEscalaInverso;
		
		glTranslatef(xTranslated, yTranslated, 0.0);
		
		glPushMatrix(); 
		//	xTranslated = (this->cx + this->raio)*fatorEscalaInverso;
		//	yTranslated = (this->cy)*fatorEscalaInverso;
	
			// Ir para direita do círculo
		//	glTranslatef(xTranslated, yTranslated, 0.0);
			
			// rotacionar o helicoptero em 90º
			glRotatef(90.0 + this->anguloGiro, 0.0, 0.0, 1.0);
			
			glPushMatrix();
				xTranslated = 0.0;
				yTranslated = this->miraHeight;
				
				glTranslatef(xTranslated, yTranslated, 0.0);
				glRotatef(this->anguloMira, 0.0, 0.0, 1.0);
				
				glPushMatrix();
					xTranslated = 0.0;
					yTranslated = -this->miraHeight;
					glTranslatef(xTranslated, yTranslated,0.0);
				
					this->tiro->desenharCircle(0.0, 0.0, 0.0);
				/*
				Add to work on linux
				glPopMatrix();
				*/
			glPopMatrix();
		glPopMatrix();
	glPopMatrix(); // Escala
}

void Tiro::movimentarParaFrente() {
	this->posX = this->posX + this->velocidadeTiro*cos(this->anguloGiro*M_PI/180 + this->anguloMira*M_PI/180);
	this->posY += this->velocidadeTiro*sin(this->anguloGiro*M_PI/180 + this->anguloMira*M_PI/180);
}

void Tiro::movimentarParaTras() {
	this->posX -= this->velocidadeTiro*cos(this->anguloGiro*M_PI/180 + this->anguloMira*M_PI/180);
	this->posY -= this->velocidadeTiro*sin(this->anguloGiro*M_PI/180 + this->anguloMira*M_PI/180);	
}

void Tiro::showValues() {
	cout << "Tiro\n";
	this->tiro->showValues();
	cout << "Velocidade Tiro: "<< this->velocidadeTiro << "\n";
}
