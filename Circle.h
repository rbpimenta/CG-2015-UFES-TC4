/*
 * Circle.h
 *
 *  Created on: 14/09/2015
 *      Author: rodrigo
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "tinyxml2.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <iostream>
#include <string>
#include <math.h>

using namespace std;
using namespace tinyxml2;

class Circle {
protected:
	float cx;
	float cy;
	float r;
	string fill;
	string id;

public:
	Circle();
	virtual ~Circle();
	void setValues(XMLElement* elem);
	void showValues();
	
	void desenharCircle(float radius, float R, float G, float B);


	// Getters and Setters
	float getCx();
	void setCx(float cx);

	float getCy();
	void setCy(float cy);

	float getR();
	void setR(float r);

	string getFill();
	void setFill(string fill);

	string getId();
	void setId(string id);
};

#endif /* CIRCLE_H_ */
