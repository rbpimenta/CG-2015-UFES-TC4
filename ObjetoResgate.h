#ifndef OBJETORESGATE_H
#define OBJETORESGATE_H

#include "Circle.h"

class ObjetoResgate
{
	public:
		Circle* dadosObjetoResgate;
		
		bool objetoResgatado;
	
	public:
		ObjetoResgate();
		~ObjetoResgate();
		
		void desenharObjetoResgate();
		void objetoFoiResgatado();
	protected:
};

#endif
