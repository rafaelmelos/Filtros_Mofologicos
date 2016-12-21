#ifndef IMAGEM_H
#define IMAGEM_H

#include "lodepng.h"
#include <inttypes.h>
/**
 * Classe para armazenar e manipular os dados referentes a uma imagem.
 */
class Imagem{
	private:
		unsigned largura;// Largura da imagem.
		unsigned altura;// Altura da imagem;
		bool colorida;
	public:
		//Matriz de pixels de um canal
		unsigned char **matriz;
		//Matriz de pixels de 4 canais
		unsigned char ***matrizmulti;
		
		Imagem(unsigned largura, unsigned altura, bool colorida);
		Imagem(const char* arquivo, bool colorida);
		~Imagem();
		
		unsigned char **AlocarMatriz();
		unsigned Largura(){return largura;}
		unsigned Altura(){return altura;}
		
		bool Ler(const char* arquivo, bool colorida);
		bool Salvar(const char* arquivo);
};

/**
 * Contrutor da classe. Aloca uma matriz de tamanho largura x altura
 * para quardar os dados da imagem que será lida.
 * largura  Largura da imagem que será lida.
 * altura 	Comprimento da imagem que será lida.
 */
Imagem::Imagem(unsigned largura, unsigned altura, bool colorida = false){
	this->largura = largura;
	this->altura = altura;
	this->colorida = colorida;
	
	if(colorida){
		matrizmulti = new unsigned char **[4];
		for(int w = 0; w < 4; w++){
			matrizmulti[w] = AlocarMatriz();
			if(NULL == matrizmulti[w]){
				return;
			} 
		}
	}else{
		matriz = AlocarMatriz();
		if(NULL == matriz){
			return;
		}
	}
}

/**
 * Alocar matriz de pixels para um canal;
 */
unsigned char **Imagem::AlocarMatriz(){
	
	unsigned char **matriz = new unsigned char *[largura];
	if(NULL == matriz){
		return NULL;
	}
	for(int i = 0; i < largura; ++i){
		matriz[i] = new unsigned char[altura];
		if(matriz[i] == NULL){
			matriz = NULL;
			return NULL;
		}
	}
	
	return matriz;
}

/**
 * Contrutor da Classe. Lê uma imagem a partir de um arquivo.
 * arquivo Nome do aquivo que contem a imagem.
 */
Imagem::Imagem(const char* arquivo, bool colorida = false){
	Ler(arquivo, colorida);
}

/**
 * Destruidor da classe. Libera a memória alocada dinamicamente
 * para a matriz de pixels.
 */
Imagem::~Imagem(){
	//Se a imagem for colorida
	if(colorida){
		//aloca uma matriz de pixels para cada canal
		for(int w = 0; w < 4; w++){
			for(int i = 0; i < largura; ++i){
				delete [](matrizmulti[w][i]);
			}
			delete [](matrizmulti[w]);
		}
		delete []matrizmulti;
	}else{
		for(int i = 0; i < largura; ++i){
			delete [](matriz[i]);
		}
		delete []matriz;
	}
}

/**
 * Le uma imagem a partir de um arquivo.
 * arquivo 	Nome do aquivo que contem a imagem.
 * return 	verdadeiro caso a imagem tenha cido carregado com sucesso,
 * 			falso caso contrario.
 */
bool Imagem::Ler(const char* arquivo, bool colorida = false){
	this->colorida = colorida;
	std::vector<unsigned char> vetor;
	
	//decode
	unsigned erro = lodepng::decode(vetor, largura, altura, arquivo);
	if(erro){
		cout << "Erro ao ler a imagem " << erro << ": " << lodepng_error_text(erro) << std::endl;
		return false;
	}
	
	//Se a imagem for colorida
	if(colorida){
		//aloca uma matriz de pixels para cada canal
		matrizmulti = new unsigned char **[4];
		for(int w = 0; w < 4; w++){
			matrizmulti[w] = AlocarMatriz();
			if(NULL == matrizmulti[w]){
				return false;
			} 
		}
		
		//Passa os valores dos pixels para a matriz aloca.
		//Cada pixel é representado por 4 bytes, ordenado por RGBARGBA...
		for(int i = 0; i < altura; ++i){
			for(int j = 0; j < largura; j++){
				for(int k = 0; k < 4; k++){
					matrizmulti[k][j][i] = vetor[4 * i * largura + 4* j + k];
				}
			}
		}
		
	}else{
		matriz = AlocarMatriz();
		if(NULL == matriz){
			return NULL;
		}
		
		//Passa os valores dos pixels para a matriz aloca.
		//Cada pixel é representado por 4 bytes, ordenado por RGBARGBA...
		for(int i = 0; i < altura; ++i){
			for(int j = 0; j < largura; j++){
				matriz[j][i] = vetor[4 * i * largura + j * 4];
			}
		}
	}
	
	return true;
}

/**
 * Salva em um arquivo a matriz de pixels em forma de imagem.
 * arquivo 	Nome do arquivo onde será salva a imagem.
 * return 	Verdadeiro se a imagem foi salva, falso caso contrario.
 */
bool Imagem::Salvar(const char* arquivo){
	std::vector<unsigned char> vetor;
	
	//aloca espaco para passar a matriz
	vetor.resize(largura * altura * 4);
	
	//Carrega o vetor com os dados da matriz de pixels
	for(int i = 0; i < altura; ++i){
		for(int j = 0; j < largura; j++){
			for(int k = 0; k < 3; k++){
				vetor[4 * i * largura + 4* j + k] = colorida ? matrizmulti[k][j][i] : matriz[j][i];
			}
			vetor[4 * i * largura + 4 * j + 3] = colorida ? matrizmulti[3][j][i] : BRANCO;
		}
	}
	
	//Encode the image
	unsigned erro = lodepng::encode(arquivo, vetor, largura, altura);

	//if there's an error, display it
	if(erro){
		cout << "Erro ao salvar a imagem " << erro << ": " << lodepng_error_text(erro) << std::endl;
		return false;
	}
	return true;
}
#endif