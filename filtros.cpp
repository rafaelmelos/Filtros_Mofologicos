#include <iostream>
using std::cout;
using std::cerr;
#include <vector>
using std::vector;
#include <algorithm>

#define PRETO 	0
#define BRANCO 255

#include "imagem.h"
#include "elementosestruturantes.h"

//Protótipos de função
Imagem * Dilatacao(Imagem *imagem, ElementoEstruturante *elemento);
Imagem * Erosao(Imagem *imagem, ElementoEstruturante *elemento);
Imagem * Mediana(Imagem *imagem, int mascara);

/**
 * Exemplo de utilização.
 */

int main(){
	//Declaração dos elementos estruturantes da dilatação
	ElementoEstruturante dilatacao1(0, 1, 1);
	ElementoEstruturante dilatacao3(1, 1);
	
	//Declaração dos elementos estruturantes da erosão
	ElementoEstruturante erosao1(1, 1);
	erosao1.matriz[0][0] = erosao1.matriz[0][2] = BRANCO;
	erosao1.matriz[2][0] = erosao1.matriz[2][2] = BRANCO;
	ElementoEstruturante erosao2(0, 1, 1);
	ElementoEstruturante erosao3(1, 1);
	erosao3.matriz[0][0] = erosao3.matriz[0][1] = BRANCO;
	erosao3.matriz[1][0] = erosao3.matriz[1][2] = BRANCO;
	erosao3.matriz[2][1] = erosao3.matriz[2][2] = BRANCO;
	
	//Carregar a imagem original
	Imagem original("imagens\\original.png");
	if(NULL == original.matriz){
		cerr << "Erro1: falha ao carregar a imagem.";
		return 1;
	}
	
	//Dilatação
	Imagem * dilatada = Dilatacao(&original, &dilatacao1);
	dilatada->Salvar("imagens\\dilatacao1.png");
	delete dilatada;
	
	dilatada = Dilatacao(&original, &dilatacao3);
	dilatada->Salvar("imagens\\dilatacao2.png");
	delete dilatada;
	
	//Erosao
	Imagem * erosao = Erosao(&original, &erosao1);
	erosao->Salvar("imagens\\erosao1.png");
	delete erosao;
	
	erosao = Erosao(&original, &erosao2);
	erosao->Salvar("imagens\\erosao2.png");
	delete erosao;
	
	erosao = Erosao(&original, &erosao3);
	erosao->Salvar("imagens\\erosao3.png");
	delete erosao;
	
	
	//Carregar a imagem com 4 canais (para mediana)
	Imagem imagemcolorida("imagens\\original.png", true);
	if(NULL == imagemcolorida.matrizmulti){
		cerr << "Erro: falha ao carregar a imagem.";
		return 1;
	}
	
	//Mediana
	Imagem * mediana = Mediana(&imagemcolorida, 3);
	mediana->Salvar("imagens\\mediana.png");
	
	delete mediana;
	
	
	return 0;
}

/**
 * Implementa o filtro morfológico da dilatação.
 * imagem 	Imagem que passara pelo filtro.
 * elemento Elemento estruturante aplicado no filtro.
 * return 	A imagem resultante do filtro ou NULL em caso de falha.
 */
Imagem * Dilatacao(Imagem *imagem, ElementoEstruturante *elemento){
	
	Imagem *imgDilata = new Imagem(imagem->Largura(), imagem->Altura());
	if(NULL == imgDilata->matriz){
		return NULL;
	}
	
	//Copia os pixels da imagem original para a imagem dilatada
	for(int i = 0; i < (imagem->Largura()); i++){
		for(int j = 0; j < (imagem->Altura()); j++){
			(imgDilata->matriz)[i][j] = (imagem->matriz)[i][j];
		}
	}
	
	unsigned largura = imagem->Largura() - elemento->largura + 1 + elemento->centrol;
	unsigned altura = imagem->Altura() - elemento->altura + 1 + elemento->centroh;
	
	//Para cada pixel da imagem original
	for(int i = elemento->centrol; i < largura; i++){
		for(int j = elemento->centroh; j < altura; j++){
			
			//Se na matriz original o pixel é preto
			if(0 == ((imagem->matriz)[i][j])){
				
				//Para cada pixel do elemento estruturante
				for(int k = 0; k < elemento->largura; k++){
					for(int l = 0; l < elemento->altura; l++){
						
						//Se o pixel do elemento estivel preto
						if(!((elemento->matriz)[k][l])){
							//Pinta de preto o pixel paralelo a ele na imagem dilatada
							(imgDilata->matriz)[i - 1 + k][j - 1 + l] = 0;
						}
						
					}
				}
				
			}
			
		}
	}
	//Retorna a nova imagem com o filto
	return imgDilata;
}

/**
 * Implementa o filtro morfológico da erosao.
 * imagem 	Imagem que passara pelo filtro.
 * elemento Elemento estruturante aplicado no filtro.
 * return 	A imagem resultante do filtro ou NULL em caso de falha.
 */
Imagem * Erosao(Imagem *imagem, ElementoEstruturante *elemento){
	
	Imagem *imgErosao = new Imagem(imagem->Largura(), imagem->Altura());
	if(NULL == imgErosao->matriz){
		return NULL;
	}
	
	//Preencher com branco toda a imagem
	for(int i = 0; i < (imagem->Largura()); i++){
		for(int j = 0; j < (imagem->Altura()); j++){
			(imgErosao->matriz)[i][j] = BRANCO;
		}
	}
	
	unsigned largura = imagem->Largura() - elemento->largura + 1 + elemento->centrol;
	unsigned altura = imagem->Altura() - elemento->altura + 1 + elemento->centroh;
	
	//Para cada pixel da imagem o riginal
	for(int i = elemento->centrol; i < largura; i++){
		for(int j = elemento->centroh; j < altura; j++){
			
			//Para cada pixel do elemento estruturante
			bool ligalPixel = true;
			for(int k = 0; k < elemento->largura; k++){
				for(int l = 0; l < elemento->altura; l++){
					
					//Se o pixel do elemento estiver preto
					//e o pixel paralelo da imagem original não estiver  
					if(!((elemento->matriz)[k][l]) && ((imagem->matriz)[i - 1 + k][j - 1 + l])){
						//O pixel não deve ser pintado de preto
						ligalPixel = false;
					}
				}
			}
			//Se o pixel pode ser pintado de preto
			if(ligalPixel){
				//Pinta o pixel de preto
				(imgErosao->matriz)[i][j] = 0;
			}
		}
	}
	//Retorna a nova imagem com o filto
	return imgErosao;
}

/**
 * Implementa um filtro de passa-baixa mediana. Cada um dos quatro canais da imagem
 * será filtrado individualmente.
 * imagem 	Imagem que será filtrada.
 * mascara 	lasrgura da mascara usada no filtro.
 * return 	imagem filtrada ou NULL em caso de falha.
 */
Imagem * Mediana(Imagem *imagem, int mascara){
	Imagem *imgMediana = new Imagem(imagem->Largura(), imagem->Altura(), true);
	if(NULL == imgMediana->matrizmulti){
		return NULL;
	}
	unsigned largura = imagem->Largura(), altura = imagem->Altura();
	
	//Para cada pixel da imagem o riginal
	for(int i = 0; i < largura; i++){
		for(int j = 0; j < altura; j++){
			//Calcula o alcanse da mascara
			int liml = i + mascara/2;
			liml = liml < largura ? liml :  largura - 1;
			
			int limh = j + mascara/2;
			limh = limh < altura ? limh :  altura - 1;
			
			vector < unsigned char > vizinho[4];
			//insere os valores dos pixels vizinhos nos vetores dos seus respectivos canais
			for(int k = (i < mascara/2 ? 0 :  i - mascara/2); k < liml; k++){
				for(int l = (j < mascara/2 ? 0 :  j - mascara/2); l < limh; l++){
					for(int w = 0; w < 4; w++){
						vizinho[w].push_back((imagem->matrizmulti)[w][k][l]);
					}
				}
			}
			
			//ordena os valores na mascara em cada canal
			for(int w = 0; w < 4; w++){
				std::sort(vizinho[w].begin(), vizinho[w].end());
			}
			
			//Se o numero de elementos na mascara for impar
			if(vizinho[0].size() % 2){
				//Para cada canal
				for(int w = 0; w < 4; w++){
					//Seta o valor da mediana;
					(imgMediana->matrizmulti)[w][i][j] = vizinho[w][vizinho[w].size()/2 + 1];
				}
			}else{//Se o numero de elementos na mascara for par
				//Para cada canal
				for(int w = 0; w < 4; w++){
					//Seta o valor da media dos valores do meio do vetor
					(imgMediana->matrizmulti)[w][i][j] = (vizinho[w][vizinho[w].size()/2]  + vizinho[w][vizinho[w].size()/2 + 1])/2;
				}
			}
		}
	}
	
	return imgMediana;
}