/*
 * Circle.cpp
 *
 *  Created on: 14/09/2015
 *      Author: rodrigo
 */

#include "Circle.h"

Circle::Circle() {
	// TODO Auto-generated constructor stub
	this->cx = 0.0;
	this->cy = 0.0;
	this->fill = "";
	this->id = "";
	this->r = 0.0;
}

Circle::~Circle() {
	// TODO Auto-generated destructor stub
}

void Circle::showValues() {
	cout << "Dados Circle:\n";
	cout << "Id = " << this->getId() << "\n";
	cout << "Cx = " << this->getCx() << "\n";
	cout << "Cy = " << this->getCy() << "\n";
	cout << "R = " << this->getR() << "\n";
	cout << "Fill = " << this->getFill() << "\n\n";
}

void Circle::setValues(XMLElement* elem) {
	// atributos do tipo rect
	float cx = 0.0; // posição em X
	float cy = 0.0; // posição em Y
	float r = 0.0; // comprimento
	string fill = "";
	string id = ""; // Identificador

	// set Id
	const char* aux = "";
	aux = (elem->Attribute("id"));
	id = aux;
	this->id = id;

	// set Cx
	elem->QueryFloatAttribute("cx", &cx);
	this->setCx(cx);

	// set Cy
	elem->QueryFloatAttribute("cy", &cy);
	this->setCy(cy);

	// set R
	elem->QueryFloatAttribute("r", &r);
	this->setR(r);

	// set Fill
	aux = elem->Attribute("fill");
	fill = aux;
	this->setFill(fill);
}

void Circle::desenharCircle (float radius, float R, float G, float B) {
int i = 0;
	int num_segments = 100;
	float twicePi = 2*M_PI;
		
	glColor3f(R, G, B);
	// Desenhar a parte interna do círculo
	glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
	glVertex3f(0.0, 0.0, 0.0); // center of circle
	   for (i = 0; i <= 20; i++)   {
			glVertex3f (
				(radius * cosf(i * twicePi / 20)),
				(radius * sin(i * twicePi / 20)),
				0.0);
		}
	glEnd();

	// Desenhar a parte da superfície do círculo
	glBegin(GL_LINE_LOOP);
		for (i = 0; i < num_segments; i++)   {
			float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle
			float x = radius * cosf(theta);//calculate the x component
			float y = radius * sinf(theta);//calculate the y component
			glVertex3f(x, y , 0.0);//output vertex
		}
	glEnd();
}

// Getters and Setters
float Circle::getCx() {
	return this->cx;
}

void Circle::setCx(float cx) {
	this->cx = cx;
}

float Circle::getCy() {
	return this->cy;
}

void Circle::setCy(float cy) {
	this->cy = cy;
}

float Circle::getR() {
	return this->r;
}

void Circle::setR(float r) {
	this->r = r;
}

string Circle::getFill() {
	return this->fill;
}

void Circle::setFill(string fill) {
	this->fill = fill;
}

string Circle::getId() {
	return this->id;
}

void Circle::setId(string id) {
	this->id = id;
}
