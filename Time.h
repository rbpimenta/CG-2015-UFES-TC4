#ifndef TIME_H
#define TIME_H

class Time
{
	public:
		// tempo da �ltima recarga, ou seja, tempo do momento que o jogador estiver no posto de combust�vel
		float tempoUltimaCarga;
		
		// tempo m�ximo de v�o, igual a vari�vel tempoDeVoo definida anteriormente
		float tempoMaximo;
		
		// tempo que sobra de v�o para o jogador
		float tempoAtual;

		// Tempo que o helicoptero esta no ar
		float tempoAr;

		// Tempo em que o helicoptero aterrisou
		float tempoAterrisagem;
	
	public:
		Time();
		~Time();
	protected:
};

#endif
