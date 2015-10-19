#ifndef POLYLINE_H
#define POLYLINE_H

#include "Ponto.h"
#include "Circle.h"
#include "tinyxml2.h"

#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;
using namespace tinyxml2;

class Polyline
{
	protected:
		string fill;
		string stroke;
		int stroke_width;
		
		
	public:
		vector<Ponto>* pontos;
		Polyline();
		~Polyline();
		void setValues(XMLElement* elem);
		void showValues();
		
		void desenharPolyline(Circle* c, float R, float G, float B);
		
		// Getters and Setters
		string getFill(){
			return this->fill;
		}
		string getStroke(){
			return this->stroke;
		}
		int getStrokeWidth(){
			return this->stroke_width;
		}
		vector<Ponto>* getPontos(){
			return this->pontos;
		}
		Ponto getPonto(float index){
			return this->pontos->at(index);
		}
};

#endif
