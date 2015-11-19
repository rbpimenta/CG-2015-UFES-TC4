#include "Helicoptero.h"

Helicoptero::Helicoptero() {
	transformacao = new Transformacao();
	
	// Inicializando velocidades
	this->velocidadeHelicoptero = 0.0;

	this->tipo = "";
	this->foiAtingido = false;

	this->tempo = new Time();
	this->temCombustivel = true;

	// valor se helicoptero tipo = 'inimigo'
	this->freqTiro = 0.0;
	this->anguloMovimentoAleatorio = 0.0;
	this->tempoUltimoDisparo = 0.0;

	// Movimenta��o
	this->posX = 0.0;
	this->posY = 0.0;
	this->anguloGiro = 0.0;
	this->escalaHelicoptero = 1.0;
	this->enableMovimento = false;
	
	this->noLimiteDaArena = false;

	// Transformacao
	this->fatorEscala = 0.0;
	float cx = 0.0;
	float cy = 0.0;
	float raio = 0.0;

	// Helices
	this->velocidadeHelices = 0.5;
	this->anguloHelices = 0.0;
	this->aceleracaoHelices = 0.5;

	// Tiros
	this->tiros = new vector<Tiro>();

	// mira
	this->anguloMira = 0.0;
	this->posMiraAnteriorX = 0.0;
	this->posMiraAnteriorY = 0.0;

	// Inicializando vari�veis do corpo
	this->color[0] = 0.0;
	this->color[1] = 0.0;
	this->color[2] = 0.0;
	this->mira = new Rectangle();
	this->corpo = new Rectangle();
	this->cauda = new Rectangle();
	this->caudaEsquerda = new Rectangle();
	this->caudaDireita = new Rectangle();
	this->centroHelice = new Circle();
	this->helices = new vector<Polyline>();
	this->dadosCircle = new Circle();
}

Helicoptero::~Helicoptero() {
}

void Helicoptero::carregarInformacoes() {
	XMLDocument* doc = new XMLDocument();

	doc->LoadFile("helicoptero.svg");

	if (doc == NULL) {
		cout << "Problema ao abrir arquivo .svg\n";
		exit(1);
	}

	XMLElement* svg = doc->FirstChildElement("svg");
	if (svg == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'svg'! Finalizando programa...\n";
		exit(1);
	}

	XMLElement* rectElem = svg->FirstChildElement("rect");

	if (rectElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'rect'! Finalizando programa...\n";
		exit(1);
	}

	// atributos do tipo rect
	string id = ""; // Identificador
	const char* aux = "";

	while (rectElem != NULL) {
		// set Id
		aux = (rectElem->Attribute("id"));
		id = aux;

		// Carregar Mira
		if (id == "Mira") {
			this->mira->setValues(rectElem);
		} else

		// Carregar Corpo
		if (id == "Corpo") {
			this->corpo->setValues(rectElem);
		} else

		// Carregar Cauda
		if (id == "Cauda") {
			this->cauda->setValues(rectElem);
		}

		// Carregar CaudaEsquerda
		if (id == "CaudaEsq") {
			this->caudaEsquerda->setValues(rectElem);
		}

		// Carregar CaudaDireita
		if (id == "CaudaDir") {
			this->caudaDireita->setValues(rectElem);
		}

		rectElem = rectElem->NextSiblingElement("rect");
	}

	// Carregando informa��es do centro da h�lice
	XMLElement* circleElem = svg->FirstChildElement("circle");

	if (circleElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'circle'! Finalizando programa...\n";
		exit(1);
	}

	this->centroHelice->setValues(circleElem);

	// Carregando informa��es da h�lice
	XMLElement* polylineElem = svg->FirstChildElement("polyline");

	if (polylineElem == NULL) {
		cout
				<< "Erro na hora de encontrar Element 'polyline'! Finalizando programa...\n";
		exit(1);
	}

	while (polylineElem != NULL) {
		Polyline* p = new Polyline();
		p->setValues(polylineElem);
		this->helices->push_back(*p);

		polylineElem = polylineElem->NextSiblingElement("polyline");
	}
}

void Helicoptero::setarValores(Circle* c) {
	/*this->dadosCircle->setCx(c->getCx());
	 this->dadosCircle->setCy(c->getCy());
	 this->dadosCircle->setFill(c->getFill());
	 this->dadosCircle->setId(c->getId());
	 this->dadosCircle->setR(c->getR());*/
	this->dadosCircle = c;

	this->posX = this->dadosCircle->getCx();
	this->posY = this->dadosCircle->getCy();
}

void Helicoptero::desenharMira() {
	float xTranslated = 0.0;
	float yTranslated = this->mira->getHeight();
	glPushMatrix();
	// mover-se para a base do corpo
	glTranslatef(xTranslated, yTranslated, 0.0);
	glRotatef(this->anguloMira, 0.0, 0.0, 1.0);
	glPushMatrix();
	xTranslated = 0.0;
	yTranslated = -this->mira->getHeight();
	glTranslatef(xTranslated, yTranslated, 0.0);

	this->mira->desenharRectangle(this->mira->getHeight(),
			this->mira->getWidth(), this->color[0], this->color[1],
			this->color[2], true);
	glPopMatrix();
	/*
	 Add to work on linux
	glPopMatrix();
	 */

}

void Helicoptero::desenharCorpo() {
	float xTranslated = 0.0;
	float yTranslated = this->mira->getHeight();
	glPushMatrix();
	glTranslatef(xTranslated, yTranslated, 0.0);
	this->corpo->desenharRectangle(this->corpo->getHeight(),
			this->corpo->getWidth(), this->color[0], this->color[1],
			this->color[2], true);
	glPopMatrix();
}

void Helicoptero::desenharCaudaDireita() {
	float xTranslated = (this->cauda->getWidth() / 2
			+ this->caudaDireita->getWidth() / 2);
	float yTranslated = this->mira->getHeight() + this->corpo->getHeight() + 20;
	glPushMatrix();
	glTranslatef(xTranslated, yTranslated, 0.0);
	this->caudaDireita->desenharRectangle(this->caudaDireita->getHeight(),
			this->caudaDireita->getWidth(), this->color[0], this->color[1],
			this->color[2], true);
	glPopMatrix();
}

void Helicoptero::desenharCaudaEsquerda() {
	float xTranslated = -(this->cauda->getWidth() / 2
			+ this->caudaEsquerda->getWidth() / 2);
	float yTranslated = this->mira->getHeight() + this->corpo->getHeight() + 20;
	glPushMatrix();
	glTranslatef(xTranslated, yTranslated, 0.0);
	this->caudaEsquerda->desenharRectangle(this->caudaEsquerda->getHeight(),
			this->caudaEsquerda->getWidth(), this->color[0], this->color[1],
			this->color[2], true);
	glPopMatrix();
}

void Helicoptero::desenharCaudaPrincipal() {
	float xTranslated = 0.0;
	float yTranslated = this->mira->getHeight() + this->corpo->getHeight();
	glPushMatrix();
	// Se movimentar para jun��o entre corpo e cauda
	glTranslatef(xTranslated, yTranslated, 0.0);

	// desenhar cauda principal
	this->cauda->desenharRectangle(this->cauda->getHeight(),
			this->cauda->getWidth(), this->color[0], this->color[1],
			this->color[2], true);
	glPopMatrix();

}

void Helicoptero::aumentarGiroHelices() {
	this->velocidadeHelices += this->aceleracaoHelices;
}

void Helicoptero::diminuirGiroHelices() {
	if (this->velocidadeHelices - this->aceleracaoHelices < 0) {
		this->velocidadeHelices = 0.0;
	} else {
		this->velocidadeHelices -= this->aceleracaoHelices;
	}

}

void Helicoptero::moverHelice() {
	if (this->anguloHelices + this->velocidadeHelices > 360) {
		this->anguloHelices = 0.0;
	} else if (this->anguloHelices + this->velocidadeHelices < 0) {
		this->anguloHelices = 0.0;
	} else {
		this->anguloHelices += this->velocidadeHelices;
	}
}

void Helicoptero::desenharHelices(Circle* c) {
	glPushMatrix();
	// Desconsiderando o giro do corpo, fazendo com que a helice gire independente
	glRotatef(this->anguloHelices - this->anguloGiro, 0.0, 0.0, 1.0);
	int i = 0;
	Polyline aux;
	for (i = 0; i < 4; i++) {
		aux = this->helices->at(i);
		aux.desenharPolyline(c, 0.0, 0.0, 1.0);
	}
	glPopMatrix();
}

void Helicoptero::desenharCentroHelice() {
	float xTranslated = 0.0;
	float yTranslated = this->mira->getHeight() + 20;
	glPushMatrix();
	glTranslatef(xTranslated, yTranslated, 0.0);
	this->centroHelice->desenharCircle(1.0, 1.0, 0.0);
	this->desenharHelices(this->centroHelice);
	glPopMatrix();
}

void Helicoptero::desenharHelicoptero() {
	if (this->foiAtingido == true) {
		return;
	}

	float comprimentoHelicoptero = this->corpo->getHeight() + 20
			+ this->caudaDireita->getHeight();

	// Definir o centro do c�rculo como a posi��o posX e posY
	float fatorEscala = (((2 * this->dadosCircle->getR())
			/ comprimentoHelicoptero) * this->escalaHelicoptero) * 0.5;

	float raio = comprimentoHelicoptero / 2;

	this->fatorEscala = fatorEscala;
	this->dadosCircle->setCx(this->posX);
	this->dadosCircle->setCy(this->posY);
	//this->cx = this->dadosCircle->getCx();
	//this->cy = this->dadosCircle->getCy();
	//this->raio = this->dadosCircle->getR();

	float fatorEscalaInverso = 1 / fatorEscala;
	float xScale = 0.0;
	float yScale = 0.0;
	float xTranslated = 0.0, yTranslated = 0.0;

/*	glPushMatrix(); // Escala
		// diminuir tamanho do helicoptero de acordo com tamanho do c�rculo
		glScalef(fatorEscala, fatorEscala, 0.0);
	
		// Na primeira vez referencia posX e posY para o centro do c�rculo
		xTranslated = this->posX * fatorEscalaInverso;
		yTranslated = this->posY * fatorEscalaInverso;
	
		// movimentar o helicoptero
		glTranslatef(xTranslated, yTranslated, 0.0);
	
		glPushMatrix();
			// rotacionar o helicoptero de acordo com o usu�rio
			glRotatef(this->anguloGiro, 0.0, 0.0, 1.0);
		
			/*xTranslated = (this->dadosCircle->getR())*fatorEscalaInverso;
			 yTranslated = 0.0;
			 
		
			xTranslated = 0.0;
			yTranslated = -(this->dadosCircle->getR()) * fatorEscalaInverso;
		
			// Ir para a parte superior do c�rculo
			glTranslatef(xTranslated, yTranslated, 0.0);
		*/
	this->transformacao->definirTransformacao(fatorEscala, this->dadosCircle->getCx(), this->dadosCircle->getCy(), this->dadosCircle->getR(), this->anguloGiro, this->anguloMira, this->mira->getHeight(), this->posX, this->posY);
	this->transformacao->iniciarTransformacao(false);
			glPushMatrix();
				xTranslated = 0.0;
				yTranslated = comprimentoHelicoptero / 2;
				glTranslatef(xTranslated, yTranslated, 0.0);
				glScalef(1 / fatorEscala, 1 / fatorEscala, 0.0);
				// Desenhar c�rculo do helicoptero
				// this->dadosCircle->desenharCircle(this->color[0], this->color[1], this->color[2]);
			glPopMatrix();
		
			this->desenharMira();
			this->desenharCorpo();
			this->desenharCaudaPrincipal();
			this->desenharCaudaDireita();
			this->desenharCaudaEsquerda();
			this->desenharCentroHelice();

	this->transformacao->fecharTransformacao();
//		glPopMatrix(); // fim direita do c�rculo
//	glPopMatrix();	// Fim escala
}

bool Helicoptero::detectarColisaoHelicoptero(Circle* c) {
	float x = this->dadosCircle->getCx();
	float y = this->dadosCircle->getCy();
	float raio = this->dadosCircle->getR();

	if (c->internoCircunferencia(x + raio, y)) {
		//cout << "Retornou limite Direito\n";
		this->posX -= this->velocidadeHelicoptero;
		return true;
	}

	if (c->internoCircunferencia(x - raio, y)) {
		//cout << "Retornou limite Esquerdo\n";
		this->posX += this->velocidadeHelicoptero;
		return true;
	}

	if (c->internoCircunferencia(x, y + raio)) {
		//cout << "Retornou limite Direito\n";
		this->posY -= this->velocidadeHelicoptero;
		return true;
	}

	if (c->internoCircunferencia(x, y - raio)) {
		//cout << "Retornou limite Esquerdo\n";
		this->posY += this->velocidadeHelicoptero;
		return true;
	}

	return false;
}

bool Helicoptero::detectarLimitesHelicopteros(
		vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador) {
	Helicoptero* h;
	int i;

	if (this->tipo == "jogador") {
		// verifica todos os inimigos
		for (i = 0; i < helicopterosInimigos->size(); i++) {
			h = &(helicopterosInimigos->at(i));
			if (h->foiAtingido == false) {
				if (h->enableMovimento == false) {
					return true;
				}
				
				if (this->detectarColisaoHelicoptero(h->dadosCircle) == true) {
					//cout << "Helicoptero colidiu!\n";
					return false;
				}
			}
		}

	} else if (this->tipo == "inimigo") {
		// verifica todos menos o inimigo em quest�o
		if (jogador->enableMovimento == true && this->detectarColisaoHelicoptero(jogador->dadosCircle)){
			return false;
		}
		
		// verificar helicopteros inimigos
		for (i = 0; i < helicopterosInimigos->size(); i++) {
			h = &(helicopterosInimigos->at(i));
			if (h->foiAtingido == false && h->identificador != this->identificador) {
				if (this->detectarColisaoHelicoptero(h->dadosCircle) == true) {
					//cout << "Helicoptero colidiu!\n";
					return false;
				}
			}
		}
	}

	return true;
}

bool Helicoptero::detectarLimitesArena(float limiteSuperior,
		float limiteInferior, float limiteEsquerdo, float limiteDireito) {
	
	float x = this->posX;
	float y = this->posY;
	float raio = this->dadosCircle->getR();

	if ((x + raio) > limiteDireito) {
		this->noLimiteDaArena = true;
		this->posX -= this->velocidadeHelicoptero;
		return false;
	}

	if ((x - raio) < limiteEsquerdo) {
		this->noLimiteDaArena = true;
		this->posX += this->velocidadeHelicoptero;
		return false;
	}

	if ((y + raio) > limiteInferior) {
		this->noLimiteDaArena = true;
		this->posY -= this->velocidadeHelicoptero;
		return false;
	}

	if ((y - raio) < limiteSuperior) {
		this->noLimiteDaArena = true;
		this->posY += this->velocidadeHelicoptero;
		return false;
	}

	this->noLimiteDaArena = false;
	return true;
}

void Helicoptero::moverParaFrente(float limiteSuperior, float limiteInferior,
		float limiteEsquerdo, float limiteDireito,
		vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador) {
	if (this->enableMovimento == true
			&& this->detectarLimitesArena(limiteSuperior, limiteInferior,
					limiteEsquerdo, limiteDireito)
			&& this->detectarLimitesHelicopteros(helicopterosInimigos,
					jogador)) {

		this->posX += this->velocidadeHelicoptero
				* cos((-90 + this->anguloGiro) * M_PI / 180);
		this->posY += this->velocidadeHelicoptero
				* sin((-90 + this->anguloGiro) * M_PI / 180);

	}
}

void Helicoptero::moverParaTras(float limiteSuperior, float limiteInferior,
		float limiteEsquerdo, float limiteDireito,
		vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador) {
	if (this->enableMovimento == true
			&& this->detectarLimitesArena(limiteSuperior, limiteInferior,
					limiteEsquerdo, limiteDireito)
			&& this->detectarLimitesHelicopteros(helicopterosInimigos,
					jogador)) {

		this->posX -= this->velocidadeHelicoptero
				* cos((-90 + this->anguloGiro) * M_PI / 180);
		this->posY -= this->velocidadeHelicoptero
				* sin((-90 + this->anguloGiro) * M_PI / 180);

	}
}

void Helicoptero::rotacionarEsquerda() {
	if (this->enableMovimento) {
		this->anguloGiro -= velocidadeHelicoptero;
	}
}

void Helicoptero::rotacionarDireita() {
	if (this->enableMovimento) {
		this->anguloGiro += velocidadeHelicoptero;
	}
}

void Helicoptero::mudarEscalaMovimento() {
	if (this->temCombustivel == false) {
		return;
	}

	if (this->escalaHelicoptero == 1.0) {
		this->escalaHelicoptero = 1.5;
		this->enableMovimento = true;
	} else {
		this->escalaHelicoptero = 1.0;
		this->enableMovimento = false;
	}
}

void Helicoptero::rotacionarMira(float x, float y) {
	if (posMiraAnteriorX > x) {
		rotacionarMiraEsquerda();
	} else if (posMiraAnteriorX < x) {
		rotacionarMiraDireita();
	}

	posMiraAnteriorX = x;
	posMiraAnteriorY = y;
}

void Helicoptero::rotacionarMiraDireita() {
	if (this->enableMovimento) {
		if (this->anguloMira > 44) {
			this->anguloMira = 45;
		} else {

			this->anguloMira += (2 * this->velocidadeHelicoptero);
		}
	}
}

void Helicoptero::rotacionarMiraEsquerda() {

	if (this->enableMovimento) {
		if (this->anguloMira < -44) {
			this->anguloMira = -45;
		} else {

			this->anguloMira -= (2 * this->velocidadeHelicoptero);
		}
	}

}

void Helicoptero::realizarTiro(Tiro* t) {
	if (enableMovimento) {
		Tiro* novoTiro = new Tiro(this->fatorEscala, (1 / this->fatorEscala),
				this->dadosCircle->getCx(), this->dadosCircle->getCy(),
				this->dadosCircle->getR(), this->anguloGiro, this->anguloMira,
				this->mira->getHeight(), this->posX, this->posY);
		novoTiro->setTiro(t);
		this->tiros->push_back(*novoTiro);
	}
}

void Helicoptero::mostrarTiros() {
	int i = 0;
	Tiro t;
	if (!this->tiros->empty()) {
		for (i = 0; i < (int) this->tiros->size(); i++) {
			t = tiros->at(i);
			t.desenharTiro();
		}
	}
}

void Helicoptero::movimentarTiros(float limiteSuperior, float limiteInferior,
		float limiteEsquerdo, float limiteDireito) {
	int i = 0;
	int totalTiros = this->tiros->size();
	int tirosExcluidos[totalTiros];

//	cout << "this->identificador = " << this->identificador << "\n";
//	cout << "totalTiros = " << totalTiros << "\n";
	Tiro* t;
	if (!this->tiros->empty()) {
		for (i = 0; i < totalTiros; i++) {
			t = &(this->tiros->at(i));
			// Verifica se o tiro ainda est� na tela
			if (t->verificarLimites(limiteSuperior, limiteInferior,
					limiteEsquerdo, limiteDireito) == true) {
				t->movimentarParaFrente();
			} else {
				this->tiros->erase(this->tiros->begin() + i);
				totalTiros--;
				i--;
			}
		}
	}
}

bool Helicoptero::verificaTiroInimigo(Helicoptero* helicopteroJogador) {
	Ponto *centro, *superior, *inferior, *esquerdo, *direito;
	Tiro* t;
	int j = 0;
	
	if (helicopteroJogador->enableMovimento == false) {
		return false;
	}
	// verificando todos os tiros do inimigo
	for (j = 0; j < this->tiros->size(); j++) {
		t = &(this->tiros->at(j));
		// Definindo pontos de verifica��o
		centro = new Ponto(t->getPosX(), t->getPosY());
		superior = new Ponto(t->getPosX(), t->getPosY() - t->tiro->getR());
		inferior = new Ponto(t->getPosX(), t->getPosY() + t->tiro->getR());
		direito = new Ponto(t->getPosX() + t->tiro->getR(), t->getPosY());
		esquerdo = new Ponto(t->getPosX() - t->tiro->getR(), t->getPosY());
		
		// verifica centro do tiro
		if (helicopteroJogador->dadosCircle->internoCircunferencia(centro->getX(),
				centro->getY()) == true
		// verifica parte supeior
				|| helicopteroJogador->dadosCircle->internoCircunferencia(
						superior->getX(), superior->getY()) == true
				//verifica parte inferior
				|| helicopteroJogador->dadosCircle->internoCircunferencia(
						inferior->getX(), inferior->getY()) == true
				// verifica lado direito
				|| helicopteroJogador->dadosCircle->internoCircunferencia(
						direito->getX(), direito->getY()) == true
				// verifica lado esquerdo
				|| helicopteroJogador->dadosCircle->internoCircunferencia(
						esquerdo->getX(), esquerdo->getY()) == true) {

			helicopteroJogador->foiAtingido = true;
			return true;
		}

		free(centro);
		free(superior);
		free(inferior);
		free(direito);
		free(esquerdo);		
		
	}
	
	return false;
}

int Helicoptero::verificaTirosJogador(vector<Helicoptero>* inimigos,
		float quantidadeInimigos) {
	int i = 0;
	int j = 0;
	Helicoptero* inimigo;
	Tiro* t;
	Ponto *centro, *superior, *inferior, *esquerdo, *direito;
	
	int quantidadeInimigosAtingidos = 0.0;

	for (i = 0; i < quantidadeInimigos; i++) {
		inimigo = &(inimigos->at(i));
		for (j = 0; j < this->tiros->size(); j++) {
			t = &(this->tiros->at(j));

			if (inimigo->foiAtingido == false) {

				// Definindo pontos de verifica��o
				centro = new Ponto(t->getPosX(), t->getPosY());
				superior = new Ponto(t->getPosX(), t->getPosY() - t->tiro->getR());
				inferior = new Ponto(t->getPosX(), t->getPosY() + t->tiro->getR());
				direito = new Ponto(t->getPosX() + t->tiro->getR(), t->getPosY());
				esquerdo = new Ponto(t->getPosX() - t->tiro->getR(), t->getPosY());
	
				// verifica centro do tiro
				if (inimigo->dadosCircle->internoCircunferencia(centro->getX(),
						centro->getY()) == true
				// verifica parte supeior
						|| inimigo->dadosCircle->internoCircunferencia(
								superior->getX(), superior->getY()) == true
						//verifica parte inferior
						|| inimigo->dadosCircle->internoCircunferencia(
								inferior->getX(), inferior->getY()) == true
						// verifica lado direito
						|| inimigo->dadosCircle->internoCircunferencia(
								direito->getX(), direito->getY()) == true
						// verifica lado esquerdo
						|| inimigo->dadosCircle->internoCircunferencia(
								esquerdo->getX(), esquerdo->getY()) == true) {
	
					inimigo->foiAtingido = true;
					quantidadeInimigosAtingidos++;
				}
	
				free(centro);
				free(superior);
				free(inferior);
				free(direito);
				free(esquerdo);
			}
		}
	}
	
	return quantidadeInimigosAtingidos;
}

void Helicoptero::atualizarCombustivel(Rectangle* postoAbastecimento) {

	int currentTime = (int) glutGet(GLUT_ELAPSED_TIME) / 1000;

	int diferenca = 0.0;

	// tempo atual sempre será a diferenca entre o tempo maximo e a diferenca acima
	if (this->enableMovimento) {
		// calcular a distancia entre os dois tempos
		diferenca = currentTime - this->tempo->tempoAr;

		this->tempo->tempoAtual = this->tempo->tempoAterrisagem - diferenca;

		// Se o tempo acabar o helicoptero aterrisa e a variavel temCombustivel � setada para false
		if (this->tempo->tempoAtual <= 0) {

			if (this->temCombustivel == true) {
				this->mudarEscalaMovimento();
				this->temCombustivel = false;
			}
		}
	} else {
		this->tempo->tempoAr = currentTime;
		this->tempo->tempoAterrisagem = this->tempo->tempoAtual;
	}

	// Situação para recarregar o combustível
	if (this->enableMovimento == false
			&& postoAbastecimento->detectarRectangle(this->dadosCircle->getCx(),
					this->dadosCircle->getCy())) {

		this->tempo->tempoUltimaCarga = currentTime;
		this->tempo->tempoAr = currentTime;
		this->tempo->tempoAtual = this->tempo->tempoMaximo;
		this->temCombustivel = true;
	}

}

int Helicoptero::resgatarObjeto(vector<ObjetoResgate>* objetosResgate) {
	int i = 0;
	ObjetoResgate* obj;
	int objetosResgatados = 0;

	for (i = 0; i < (int) objetosResgate->size(); i++) {
		obj = &(objetosResgate->at(i));
		if (obj->objetoResgatado == false && obj->dadosObjetoResgate->internoCircunferencia(this->posX,
				this->posY) == true) {
			obj->objetoFoiResgatado();
			objetosResgatados += 1;
		}
	}
	return objetosResgatados;
}

void Helicoptero::definirCor(float R, float G, float B) {
	this->color[0] = R;
	this->color[1] = G;
	this->color[2] = B;

}

void Helicoptero::movimentoAleatorio (float limiteSuperior, float limiteInferior, float limiteEsquerdo, float limiteDireito, vector<Helicoptero>* helicopterosInimigos, Helicoptero* jogador){
	if (this->noLimiteDaArena) {
		this->anguloGiro += (rand() % 720); 
	}
	this->moverParaFrente(limiteSuperior, limiteInferior, limiteEsquerdo, limiteDireito, helicopterosInimigos, jogador);
}

void Helicoptero::efetuarTiroInimigo (Tiro* t) {
	
	float currentTime = glutGet(GLUT_ELAPSED_TIME);
	float diferenca = currentTime - tempoUltimoDisparo;

	// meio em meio segundo o helicoptero vai atirar
	float tempoLimite = (1/this->freqTiro);
	
	if (diferenca >= tempoLimite) {	
		tempoUltimoDisparo = currentTime;
		if (this->foiAtingido == false) {
			this->realizarTiro(t);
		}
	}
}
