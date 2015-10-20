#ifndef TIME_H
#define TIME_H

class Time
{
	public:
		// tempo da última recarga, ou seja, tempo do momento que o jogador estiver no posto de combustível
		int tempoUltimaCarga; 
		
		// tempo máximo de vôo, igual a variável tempoDeVoo definida anteriormente
		int tempoMaximo;
		
		// tempo que sobra de vôo para o jogador
		int tempoAtual;
	
	public:
		Time();
		~Time();
	protected:
};

#endif
