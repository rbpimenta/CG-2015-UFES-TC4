#include "Polyline.h"

Polyline::Polyline()
{
	this->fill = "";
	this->stroke = "";
	this->stroke_width = 0;
	pontos = new vector<Ponto>();
}

Polyline::~Polyline()
{
}

void split(const string& s, char c,
           vector<string>& v) {
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

void Polyline::setValues(XMLElement* elem) {
	// atributos do tipo rect
	const char* aux = "";

	aux = elem->Attribute("fill");
	this->fill = aux;
	
	aux = elem->Attribute("stroke");
	this->stroke = aux;
	
	elem->QueryIntAttribute("stroke-width", &this->stroke_width);
	
	string strPontos = elem->Attribute("points");
	
	vector<string> vPontos;
	split (strPontos, ' ', vPontos);
	
	float x = 0.0, y = 0.0;
	vector<string> vPonto;
	
	int i = 0;
	string auxPontos;
	
	while (i < vPontos.size()) {
		split (vPontos.at(i), ',', vPonto);
		
		auxPontos = vPonto.at(0);
		x = (float) atof (auxPontos.data());

		
		auxPontos = vPonto.at(1);
		y = (float) atof (auxPontos.data());

		Ponto* p = new Ponto(x, y);
		pontos->push_back(*p);
		vPonto.clear();
		i++;
	}
}

void Polyline::showValues() {
	cout << "Mostrando valores Polyline\n";
	cout << "x | y\n";
	int i = 0;
	for (i = 0; i < 4; i++) {
		cout << this->getPonto(i).getX() << " " <<this->getPonto(i).getY() << "\n";
	}
	
}

void Polyline::desenharPolyline(Circle* c, float R, float G, float B) {
	
	int i = 0;
	float x = 0.0, y = 0.0;
	
	float cx = c->getCx();
	float cy = c->getCy();
	
	glColor3f(R,G,B);
	glBegin(GL_QUADS);
		for (i = 0; i < 4; i++) {
			x = this->getPonto(i).getX() - cx;
			y = this->getPonto(i).getY() - cy;
			glVertex2f(x, y);
		}
	glEnd();
	
	// Contorno
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
		for (i = 0; i < 4; i++) {
			x = this->getPonto(i).getX() - cx;
			y = this->getPonto(i).getY() - cy;
			glVertex2f(x, y);
		}
	glEnd();
}

