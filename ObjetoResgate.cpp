#include "ObjetoResgate.h"

ObjetoResgate::ObjetoResgate()
{
	this->dadosObjetoResgate = new Circle();
	this->objetoResgatado = false;
}

ObjetoResgate::~ObjetoResgate()
{
}

void ObjetoResgate::desenharObjetoResgate() {
	this->dadosObjetoResgate->desenharCircleSemTransformacao(0.0, 0.0, 1.0);
}

void ObjetoResgate::objetoFoiResgatado() {
	this->objetoResgatado = true;
}
