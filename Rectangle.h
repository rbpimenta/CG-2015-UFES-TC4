/*
 * Arena.h
 *
 *  Created on: 14/09/2015
 *      Author: rodrigo
 */

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <iostream>
#include "tinyxml2.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>

using namespace std;
using namespace tinyxml2;

class Rectangle {
protected:
	float x; // posição em X
	float y; // posição em Y

	float width; // comprimento
	float height; // altura

	string fill;
	int stroke_width; // Comprimento da linha
	string stroke;

	string id; // Identificador

public:
	Rectangle();
	virtual ~Rectangle();
	void showValues();
	void setValues(XMLElement* elem);
	
	void desenharRectangle(float height, float width, float R, float G, float B, bool desenharContorno);
	void desenharRectangle (float R, float G, float B, bool desenharContorno);
	void desenharContorno (float R, float G, float B);
	bool detectarRectangle (float x, float y);

	// Getters and Setters
	float getX ();
	void setX (float x);
	float getY ();
	void setY (float y);

	float getWidth ();
	void setWidth (float width);
	float getHeight ();
	void setHeight (float height);

	string getFill ();
	void setFill (string fill);
	int getStrokeWidth ();
	void setStrokeWidth (int stroke_width);
	string getStroke ();
	void setStroke (string stroke);
	string getId ();
	void setId (string id);

};

#endif /* RECTANGLE_H_ */
