#ifndef TIME_H
#define TIME_H

class Time
{
	public:
		// tempo da �ltima recarga, ou seja, tempo do momento que o jogador estiver no posto de combust�vel
		int tempoUltimaCarga; 
		
		// tempo m�ximo de v�o, igual a vari�vel tempoDeVoo definida anteriormente
		int tempoMaximo;
		
		// tempo que sobra de v�o para o jogador
		int tempoAtual;
	
	public:
		Time();
		~Time();
	protected:
};

#endif
