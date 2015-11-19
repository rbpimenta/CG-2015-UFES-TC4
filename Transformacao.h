#ifndef TRANSFORMACAO_H
#define TRANSFORMACAO_H

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

class Transformacao
{
	public:
		float fatorEscala;
		float fatorEscalaInverso;
		float cx;
		float cy;
		float raio;
		float anguloGiro;
		float anguloMira;
		float height;
		float posX;
		float posY;
		
		Transformacao();
		Transformacao(float fatorEscala, float cx, float cy, float raio, float anguloGiro, float anguloMira, float height, float posX, float posY);
		void definirTransformacao (float fatorEscala, float cx, float cy, float raio, float anguloGiro, float anguloMira, float height, float posX, float posY);
		void iniciarTransformacao(bool ehTiro);
		void fecharTransformacao();
		~Transformacao();
		
	protected:
		
		
};

#endif
