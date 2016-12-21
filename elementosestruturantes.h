#ifndef ELEMENTO_ESTRUTURANTE_H
#define ELEMENTO_ESTRUTURANTE_H

#define PRETO 	0
#define BRANCO 255

/**
 * Classe para representar um elemento estruturante.
 * Exemplo de declaração:
 * | |X| | ElementoEstruturante erosao1(1, 1);
 * |X|X|X| erosao1.matriz[0][0] = erosao1.matriz[0][2] = BRANCO;
 * | |X| | erosao1.matriz[2][0] = erosao1.matriz[2][2] = BRANCO;
 */

struct ElementoEstruturante{
	public:
		int largura, altura; //Largura e altura do elemento
		int centrol, centroh; //Coordenadas do cento do elemento
		uint8_t **matriz;//Matriz de pixels
		
		ElementoEstruturante(int centroh, int centrol, int altura, int largura);
		~ElementoEstruturante();
};

/**
 * Construtor da classe. Aloca uma matriz de tamanho largura x altura e seta os pixels
 * para preto.
 */
ElementoEstruturante:: ElementoEstruturante(int centroh, int centrol, int altura = 3, int largura = 3){
	this->centrol = centrol;
	this->centroh = centroh;
	this->largura = largura;
	this->altura = altura;
	
	//alocar matriz de pixels
	matriz = new uint8_t *[largura];
	for(int i = 0; i < largura; ++i){
		matriz[i] = new uint8_t[altura];
		for(int j = 0; j < altura; j++){
			matriz[i][j] = 0;
		}
	}
}

/**
 * Destruidor da classe. Libera a memória aloca dinamicamente
 * para a matriz de pixels.
 */
ElementoEstruturante::~ElementoEstruturante(){
	for(int i = 0; i < largura; ++i){
		delete [](matriz[i]);
	}
	delete []matriz;
}

#endif