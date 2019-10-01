/*
  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__

  AO PREENCHER ESSE CABEÇALHO COM O MEU NOME E O MEU NÚMERO USP, 
  DECLARO QUE SOU O ÚNICO AUTOR E RESPONSÁVEL POR ESSE PROGRAMA. 
  TODAS AS PARTES ORIGINAIS DESSE EXERCÍCIO PROGRAMA (EP) FORAM 
  DESENVOLVIDAS E IMPLEMENTADAS POR MIM SEGUINDO AS INSTRUÇÕES DESSE EP
  E QUE PORTANTO NÃO CONSTITUEM PLÁGIO. DECLARO TAMBÉM QUE SOU RESPONSÁVEL
  POR TODAS AS CÓPIAS DESSE PROGRAMA E QUE EU NÃO DISTRIBUI OU FACILITEI A
  SUA DISTRIBUIÇÃO. ESTOU CIENTE QUE OS CASOS DE PLÁGIO SÃO PUNIDOS COM 
  REPROVAÇÃO DIRETA NA DISCIPLINA.

  Nome: Gabriel Henrique Pinheiro Rodrigues
  NUSP:112.216-47

  imagem.c

  Referências: Com exceção das rotinas fornecidas no esqueleto e em sala
  de aula, caso você tenha utilizado alguma refência, liste-as abaixo
  para que o seu programa não seja considerada plágio.
  Exemplo:

  - função mallocSafe copiada de: 

  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html

  \__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__\__
*/

#include <stdio.h>  /* fprintf(), printf() */
#include <stdlib.h> /* malloc(), free(),  */
#include <math.h>   /* sqrt() */

#include "imagem.h" /* LIMIAR, Byte, Imagem, CelPixel, CelRegiao ... 
                       mallocImagem(), freeImagem(), freeRegioes()
                       copieImagem(), pinteImagem(), segmenteImagem()
                       pinteRegiao(), pinteRegioes(), AVISO(), getPixel()
                     */

#include "cores.h"  /* BACK_GROUND, NUM_CORES, cores[0..NUM_CORES] */


/*-------------------------------------------------------------
  constantes
*/
#define EXIT_FAILURE_MALLOC -1

/*-------------------------------------------------------------
  macros
*/
#define NORMA(x,y) sqrt(x*x + y*y) 

/*-------------------------------------------------------------
  Funcoes locais que devem ser escritas
*/

static void 
setPixel(Imagem *img, int col, int lin, Byte cor[]);

static Bool
pixelBorda(Imagem *img, int limiar, int col, int lin);

static int
pixelsRegiao(Imagem *img, int limiar, int col, int lin, CelRegiao *regiao);

/*-------------------------------------------------------------
  Funcoes locais que ja estao escritas  
*/

static void *
mallocSafe(size_t nbytes);

static double 
luminosidadePixel(Imagem *img, int col, int lin);

/*-------------------------------------------------------------
  mallocImagem
  
  Recebe inteiros WIDTH e HEIGHT e retorna um ponteiro para uma
  estrutura (tipo Imagem) que representa uma imagem com HEIGHT
  linhas e WIDTH colunas (HEIGHT x WIDTH pixels).

  Cada pixel da imagem e do tipo Pixel.
   
  Esta funcao utiliza a funcao mallocSafe.
*/
Imagem* mallocImagem(int width, int height){
	Imagem *img = (Imagem*) mallocSafe(sizeof(Imagem));
	img->width = width;
	img->height = height;
	//Aloca a matriz de pixels 
	img->pixel = (Pixel**) mallocSafe(height*sizeof(Pixel*));
	for (int i=0; i< height; i++) {
		img->pixel[i] = (Pixel*) mallocSafe(width*sizeof(Pixel));
	}
	return img;
}
/*-------------------------------------------------------------
  freeImagem
  
  Recebe um ponteiro IMG para um estrutura que representa uma 
  imagem  e libera a memoria utilizada pela estrutura.
  
  Esta funcao utiliza a funcao free.
*/
void freeImagem(Imagem *img){
   //Desalocando os ponteiros que apontam para as colunas, essas que são vetores
   for (int i=0; i < (img->height); i++) {
     free(img->pixel[i]);
   }
   //Desaloca o struct Imagem
   free(img);
}
/*-------------------------------------------------------------
  freeRegioes
  
  Recebe um ponteiro INIREGIOES para uma lista de regioes de
  uma imagem e libera a memoria aloca pelas celulas dessa lista.

  Esta função também libera a memoria alocada pelas celulas 
  da lista de pixels de cada regiao.
*/

void freeRegioes(CelRegiao *iniRegioes){   
	//Apontador auxiliar de regiões
	CelRegiao* tpRegioes = iniRegioes;
	//Apontador auxiliar de células pixel
	CelPixel* tpCelPixel; 

	while(tpRegioes != NULL){
		//O apontador auxiliar de células pixel começa apontando para onde o inipixels aponta
		tpCelPixel = iniRegioes->iniPixels;

		while(tpCelPixel != NULL){
			//Aponta para a próxima célula
			tpCelPixel = tpCelPixel->proxPixel;
			//Desaloca a célula anterior
			free(iniRegioes->iniPixels);
			iniRegioes->iniPixels = tpCelPixel;
		}
		//Aponta a próxima região
		tpRegioes = tpRegioes->proxRegiao;
		//Desaloca a região
		free(iniRegioes);
		iniRegioes = tpRegioes;
	}
}
/*-------------------------------------------------------------
  copieImagem 

  Recebe ponteiros DESTINO e ORIGINAL para estruturas que 
  representam imagems.
 
  A funcao copia a imagem ORIGEM sobre a imagem DESTINO. 
   
  A imagem DESTINO já deve ter sido criada antes da chama da
  funcao.  Assim, a memória utilizada por DESTINO ja deve ter
  sido _totalmente_ alocada.

*/
void copieImagem(Imagem *destino, Imagem *origem){
	for (int i=0; i < origem->height ; i++) {
		for (int j=0; j< origem->width; j++) {
			//Copia o pixel da origem para o destino
			destino->pixel[i][j] = origem->pixel[i][j];
		}
	}
}
/*-------------------------------------------------------------
  getPixel

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e uma posicao [LIN][COL] de um pixel.
 
  A funcao retorna o pixel da posicao [LIN][COL].

  Esta funcao e usada no modulo graphic para exibir a imagem
  corrente em uma janela.
*/
Pixel getPixel(Imagem *img, int col, int lin){	
	return img->pixel[lin][col];   
}
/*-------------------------------------------------------------
  setPixel
/
  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem, uma posicao [LIN][COL] de um pixel e uma COR.

  A funcao pinta o pixel da posicao [LIN][COL] da cor COR.

*/
static void setPixel(Imagem *img, int col, int lin, Byte cor[]){ 
	//Alter a cor de um pixel, mudando suas componentes (RGB)
	for (int i=0; i< 3; i++) {
	   img->pixel[lin][col].cor[i] = cor[i];	
	}
}
/*-------------------------------------------------------------
  pinteImagem

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e uma COR.

  A funcao pinta todos os pixels da imagem IMG da cor COR.

  Esta funcao deve utilizar a funcao setPixel.
*/ 
void pinteImagem(Imagem *img, Byte cor[]){
	for (int i=0; i < img->height; i++){
		for (int j=0; j < img->width; j++){
			//Pinta cada pixel de uma imagem
			setPixel(img,j,i,cor);
		}
	}
}
/*------------------------------------------------------------- 
   pinteRegioes

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um ponteiro INIREGIOES para uma lista de regioes
   da imagem e uma opcao BORDA que tera o valor TRUE or FALSE.

   Se BORDA == FALSE a funcao pinta os pixels de cada regiao da
       lista INIREGIOES de uma cor "diferente" (uma mesma cor
       para todos os pixels de uma regiao).

   Se BORDA == TRUE a funcao pinta os pixels de cada regiao de
       __borda__ da lista INIREGIOES de uma cor "diferente".
       (uma mesma cor para todos os pixels de uma regiao).

   Para fazer o seu servico a funcao percorre a lista INIREGIOES
   e para cada regiao (dependendo da opcao BORDA) percorre a sua
   lista de pixels pintando-os de uma cor.

   Para pintar as regioes de uma cor "diferente" a funcao utiliza
   'ciclicamente' as cores na tabela cores[0..NUM_CORES-1]
   (cores.h):

           cores[0] eh uma cor
           cores[1] eh outra cor
           cores[2] eh outra cor 
           ...
           cores[NUM_CORES-1] eh outra cor. 

   A funcao tambem atualiza o campo _cor_ das celulas da lista
   INIREGIOES que representam regioes que foram pintadas.
*/
void pinteRegioes(Imagem *img, CelRegiao *iniRegioes, Bool borda){ 
	//Ponteiro auxiliar que aponta para regiões
	CelRegiao *tpRegiao = iniRegioes;
	//Ponteiro auxiliar que aponta para celulas pixel
	CelPixel *tpCelPixel = tpRegiao->iniPixels;
	//índice para escolha de cores das bordas
	int indCoresB=rand()%32;
	//índice para escolha de cores das não bordas
	int indCoresNB=rand()%32;

	while(tpRegiao != NULL){
		tpCelPixel = tpRegiao->iniPixels;
		//Atualiza a informação de cor da região
		for (int i=0; i<3; i++) {
			/*Atualiza a cor da região, se for de pixel de borda utiliza o índice indCoresB,
			caso contrário utiliza indCoresNB*/
			tpRegiao->cor[i] = cores[((tpRegiao->borda==TRUE)?indCoresB:indCoresNB)][i];
		}
		while(tpCelPixel != NULL){
			setPixel(img, tpCelPixel->col, tpCelPixel->lin, cores[((tpRegiao->borda == TRUE) ? indCoresB : indCoresNB)]);
			//Vai para a próxima célula pixel
			tpCelPixel = tpCelPixel->proxPixel;
		}
		/*Muda o valor dos índices de cores*/
		(tpRegiao->borda==TRUE) ? (indCoresB = (indCoresB+1)%NUM_CORES) : (indCoresNB = (indCoresNB+1)%NUM_CORES);
	
		//Vai para a pŕoxima região
		tpRegiao = tpRegiao->proxRegiao;
	}
}
/*-------------------------------------------------------------
   repinteRegiao

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, uma posicao [LIN][COL] de um pixel da imagem e
   uma cor COR.

   A funcao repinta todos os pixels da imagem IMG que estao na
   regiao do pixel [LIN][COL] com a cor COR. 

   Para isto a funcao percorre a lista dos pixels que estao 
   na mesma regiao de [LIN][COL] pintando-os de COR.

   A funcao tambem atualiza o campo _cor_ da celula que
   representa a regiao a que o pixel [LIN][COL] pertence.
*/
void repinteRegiao(Imagem *img, int col, int lin, Byte cor[]){   
	//Acha o pixel corresponde a coordenada [lin][col]
	Pixel pixel = getPixel(img, col, lin);
	//Ponteiro auxiliar de CelPixel
	CelPixel *tpC = pixel.regiao->iniPixels;

	//Altera a cor da região a qual o pixel pertence
	for (int i=0; i < 3; i++) {
		pixel.regiao->cor[i] = cor[i];    
	}
	//Pinta enquanto o ponteiro auxiliar não é nulo
	while(tpC != NULL){
		//Pinta o pixel
		setPixel(img,tpC->col,tpC->lin,cor);
		//Ponteiro auxiliar aponta para o próximo pixel
		tpC = tpC->proxPixel;
	}
}
/*------------------------------------------------------------- 
   repinteRegioes

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um ponteiro INIREGIOES para uma lista de regioes,
   uma posicao [LIN][COL] de um pixel e uma cor COR.

   A funcao repinta os pixels da imagem IMG de 

       cada regiao que tem a mesma cor que a regiao 
       do pixel [LIN][COL] com a cor COR.

   Para isto a funcao percorre a lista INIREGIOES e, 
   para cada regiao que tem a mesma cor que a _regiao_ do pixel 
   [LIN][COL], percorre a sua lista pixels repintando-os 
   de COR.

   A funcao tambem atualiza o campo _cor_ das celulas da lista
   INIREGIOES que representam regioes que foram repintadas.
*/
void repinteRegioes(Imagem *img, CelRegiao *iniRegioes, int col, int lin, Byte cor[]){   

   Pixel pixel = getPixel(img, col, lin);
   CelRegiao *tpReg = iniRegioes;
    
   while(tpReg != NULL){
		/*Se a cor do pixel de entrada for igual ao da cor da região, então atualiza
		a cor da região com a cor de entrada*/ 
		if(pixel.cor[0] == tpReg->cor[0] && pixel.cor[1] == tpReg->cor[1] && pixel.cor[2] == tpReg->cor[2]){
			//Atualiza a cor da regiao pintada
			for (int i=0; i< 3; i++) {
				tpReg->cor[i] = cor[i];
			}
			//Repinta a região com a cor de entrada
			repinteRegiao(img,col,lin,cor);
		}
		//Passa para a próxima região
		tpReg = tpReg->proxRegiao;
   }    
}
/*------------------------------------------------------------- 
   pixelBorda

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, um posicao [LIN][COL] de um pixel da imagem
   e um valor LIMIAR.

   A funcao retorna TRUE se o pixel [LIN][COL] for de borda
   em relacao ao valor LIMIAR.

   Esta funcao utiliza a funcao luminosidadePixel().
*/
static Bool pixelBorda(Imagem *img, int limiar, int col, int lin){
	//Cria os gradientes X e Y	
	int gX,gY;
	gX=gY=0;
	/*Vetor que contém os coeficientes que serão multiplicados pela
	função luminosidadePixel*/
	int operacoes[] = {1,2,1,-1,-2,-1};
	//Vetor que contém os escalares que são somados a coluna
	int adicoesCol[] = {1,1,1,-1,-1,-1};
	//Vetor que contéßm os escalares que somam a linha
	int adicoesLin[] = {-1,0,1,-1,0,1};

	for (int i=0; i< 6; i++) {
		//Checa se o pixel analisado está dentro do escopo da imagem
		if(lin+adicoesLin[i] >= 0 && col+adicoesCol[i] >= 0 && lin+adicoesLin[i] < img->height && col+adicoesCol[i] < img->width){
			//Cálculo do gradiente X
			gX += operacoes[i] * luminosidadePixel(img, col+adicoesCol[i], lin+adicoesLin[i]);
		}
		/*O gradienteY ocorre de maneira análoga ao GX, por isso adicoesLin[i] é adicionado a coluna
		e adicoesCol[i] é adicionado a linha */
		if(lin+adicoesCol[i] >= 0 && col+adicoesLin[i] >= 0 && lin+adicoesCol[i] < img->height && col+adicoesLin[i] < img->width){
			//Cálculo do gradiente Y
			gY += operacoes[i]* luminosidadePixel(img, col+adicoesLin[i], lin+adicoesCol[i]);
		}
	}
	/*Calcula a norma, se essa for maior que o limiar, o pixel é de borda, logo
	a função retorna true, e false caso contrário*/
	if(NORMA(gX,gY) > limiar){
		return TRUE;
	}
	else{
		return FALSE;
	}
}
/*-------------------------------------------------------------
  segmenteImagem

  Recebe um ponteiro IMG para uma estrutura que representa 
  uma imagem e um inteiro LIMIAR. 

  A funcao retorna um ponteiro para o inicio de uma lista
  de regioes da imagem (em relacao ao valor LIMIAR). 

  Cada pixel da imagem deve pertencem a uma, e so uma,
  regiao. Essas regioes constituem o que se chama de uma
  _segmentacao_ da imagem.

  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
  Regioes
  --------

  Cada regiao da imagem deve ser formada: 
 
      -  _apenas_ por pixels de uma _mesma_ regiao de borda  ou 
      -  _apenas_ por pixels de uma _mesma_ regiao da imagem 
         limitada por: 

          . pixels de borda ou
          . pela fronteira da imagem. 

  Lista de regioes
  ----------------
 
  Cada celula da lista de regioes devolvida e do tipo CelRegiao.
  Os campos de cada nova celula que devem ser preenchidos pela 
  funcao sao: 
 
      - nPixels: contem o numero de pixels na regiao (pixelsRegiao())
      - borda: indica se a regiao e de borda (pixelBorda())
      - iniPixels: ponteiro para o inicio da lista de pixels que 
            formam a regiao (pixelsRegiao())
      - proxRegiao: ponteiro para proxima celula 
            
  Em particular, esta funcao nao deve preencher o campo 

      - cor: cor dos pixels da regiao,

  (Esse tarefa sera feita pela funcao pinteRegioes().)

  Lista de pixels de uma regiao
  -----------------------------

  Para obter a lista de pixels em uma mesma regiao, esta funcao
  deve utilizar a funcao pixelsRegiao() que cria e devolve a
  lista de pixels de uma mesma regiao.

  O servico feito pela funcao pixelsRegiao() sera utilizado 
  para atribuir os valores dos campos nPixels e iniPixels de
  cada celula da lista de regioes.

  Ponteiros de pixels para regioes
  --------------------------------
  
  O campo _regiao_ de cada pixel [lin][col] da imagem IMG devera
  ser utilizado para indicar se o pixel [lin][col] ja pertencem a
  uma regiao (ou, utilizandi outro jargao, se ja foi ou nao
  _visitado_):

     - IMG->pixel[lin][col].regiao == NULL 
       
       indica que o pixel [lin][col] ainda nao foi atribuido a
       uma regiao (= nao foi visitado)

  Pixels de borda
  ---------------

  Para decidir se um pixel [lin][col] eh ou nao de borda devera
  ser utilizada a funcao pixelBorda(). 

  Para fazer o seu servico a funcao pixelBorda() utiliza o valor
  LIMIAR.  Um pixel eh ou nao considerado de borda em relacao ao
  dado LIMIAR.

  mallocSafe
  ----------

  Esta funcao deve utilizar a funcao mallocSafe() para criar cada
  celula da lista de regioes.

*/
CelRegiao* segmenteImagem(Imagem *img, int limiar){
	
	//Ponteiro que aponta para a primeira célula região
	CelRegiao *celR = (CelRegiao *) mallocSafe(sizeof(CelRegiao));
	//Ponteiro auxiliar que aponta para a célula região
	CelRegiao *celAux = celR;
	//Ponteiro auxiliar para a criação de células região
	CelRegiao *nova;

	//Reseta a região inicial de todos os pixels da imagem
	for (int i=0; i< img->height; i++) {
		for (int j=0; j< img->width; j++) {
			img->pixel[i][j].regiao = NULL; 
		}
	}

	//Preenche a primeira celula região
	celR->borda = pixelBorda(img,limiar,0,0);
	celR->proxRegiao = NULL;
	celR->nPixels = 0;
	celR->nPixels = pixelsRegiao(img,limiar,0,0,celR);
	
	//Executa enquanto houver pixels sem região
	for (int i=0; i< img->height; i++) {
		for (int j=0; j< img->width; j++) {
			/*Se o pixel não pertence a nenhuma região, então uma região é criada para ele*/
			if(img->pixel[i][j].regiao == NULL){
				//Cria e preenche uma célula região
				nova = (CelRegiao *) mallocSafe(sizeof(CelRegiao));
				nova->borda = pixelBorda(img,limiar,j,i);
				nova->nPixels=0;
				nova->nPixels = pixelsRegiao(img,limiar,j,i,nova);
				nova->proxRegiao = NULL;
				/*Ponteiro auxiliar direciona a célula anterior para apontar para a célula criada*/
				celAux->proxRegiao = nova;
				celAux = celAux->proxRegiao;
			}
		}
	}
	return celR;
}
/*------------------------------------------------------------- 
  pixelsRegiao

  Recebe 
   
      - um  ponteiro IMG para uma estrutura que representa 
        uma imagem
      - uma inteiro LIMIAR 
      - uma posicao [LIN][COL] de um pixel, 
      - um  ponteiro REGIAO para uma celula que representa
        uma regiao da imagem cuja lista de pixels Regiao->iniPixels 
        esta sendo construida (recursivamente pela funcao).

  Visitando pixels a partir de posicao [LIN][COL], a funcao
  insere na lista REGIAO->iniPixels novas celulas correspondentes
  a pixels que ainda nao pertencem a regiao alguma (no jargao
  popular, ainda nao foram visitados pela funcao) e que estao na
  regiao representada por REGIAO.

  A funcao retorna o numero de (novas) celulas inseridas na lista
  REGIAO->iniPixels.

  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 
  Recursao 
  --------
   
  Os pixels inseridos na lista REGIAO->iniPixels devem ser obtidos
  RECURSIVAMENTE visitando-se (=examinando-se)

      - o pixel da posicao [LIN][COL]
 
      - os vizinhos do pixel da posicao [LIN][COL] que sao 
        do mesmo tipo de REGIAO->borda (borda ou nao borda) 
        e ainda nao foram visitados 

      - os vizinhos dos vizinhos da posicao [LIN][COL] que sao do
        mesmo tipo de REGIAO->borda (borda ou nao borda) e ainda
        nao foram visitados

      - os vizinhos, dos vizinhos, dos vizinhos ...

      - ...
 
  Vizinhanca de um pixel
  ----------------------

  O conjunto de vizinhos de um pixel depende do tipo da regiao
  (valor de REGIAO->borda) e da sua posicao (canto ou centro ou
  ...).

  Se REGIAO-BORDA == FALSE, os vizinhos de um pixel sao os pixels 
  de cima, de baixo, da esquerda e direita (vizinha da torre no 
  EP4 de MAC2166, edicao 2014). 
  Nessa caso um pixel pode ter ate 4 vizinhos.

  Se REGIAO-BORDA == TRUE, os vizinhos de um pixel sao os pixels
  de cima, de baixo, da esquerda e direita e das quatro diagonais
  (vizinha do rei no EP4 de MAC2166, edicao 2014).  

  Nessa caso um pixel pode ter ate 8 vizinhos.

  Regiao 
  ------

  O tipo dos pixels que devem ser inseridos em REGIAO->iniPixels
  depende do valor de REGIAO->borda:

     TRUE:  os pixels     devem ser de borda
     FALSE: os pixels nao devem ser de borda.

  Cada celula da lista REGIAO->iniPixels eh do tipo CelPixel.
  Assim, os campos de cada nova celula a serem preenchidos sao: 
 
      - col, lin: [lin][col] e posicao do pixel na imagem 
      - proxPixel: ponteiro para a proxima celula

  Ponteiros de pixels para regioes
  --------------------------------
  
  O campo _regiao_ de cada pixel [lin][col] da imagem IMG devera
  ser utilizado para indicar se o pixel [lin][col] ja pertencem a
  uma regiao (ja foi ou nao visitado):

     - IMG->pixel[lin][col].regiao == NULL 
       
       indica que o pixel [lin][col] ainda nao foi atribuido a
       uma regiao (= nao foi visitado)

     - IMG->pixel[lin][col].regiao != NULL 

       significa que o pixel [lin][col] esta na regiao
       correspondente a celula IMG->pixel[lin][col].regiao da
       lista de regioes (= ja foi visitado).

  Assim que um pixel [lin][col] e inserido em uma regiao o 
  seu campo regiao deve ser atualizado.

  Pixels de borda
  ---------------

  Para decidir se um pixel [lin][col] eh ou nao de borda devera
  ser utilizada a funcao pixelBorda(). 

  Para fazer o seu servico a funcao pixelBorda() utiliza o valor
  LIMIAR.  Um pixel eh ou nao considerado de borda em relacao ao
  dado LIMIAR.

  mallocSafe
  ----------

  Esta funcao deve utilizar a funcao mallocSafe para criar cada
  celula da lista de pixels.

*/
static int pixelsRegiao(Imagem *img, int limiar, int col, int lin, CelRegiao *regiao){ 
    //Vetor de posição
    int vetorPosicao[] = {-1,0,1};
    //Vetor de posição para sem borda
    int vetorPosSemBord[] = {-1,1,0,0};
    //Apontador que aponta para a célula recém criada
    CelPixel *nova;

   /*Se o pixel analisado tiver região nula e for do mesmo tipo de borda que a região(borda ou não),
	então o pixel é adicionado ao celpixel e colocado na lista de celPixels da região de entrada*/
   if(img->pixel[lin][col].regiao == NULL && (pixelBorda(img,limiar,col,lin) == regiao->borda)){
		//Cria um celPixel
		nova = (CelPixel*) mallocSafe(sizeof(CelPixel));
		//Adiciona as informações de celPixel
		nova->lin = lin;
		nova->col = col;
		nova->proxPixel = NULL;
		
		/*Se a quantidade de pixels de uma região for nula, regiao->iniPixels passa a apontar
		para a célula recém criada*/
		if(regiao->nPixels == 0){
			regiao->iniPixels = nova;
		}
		/*Caso já haja pixels na região, a nova célula aponta para onde regiao->iniPixels aponta
		e regiao->iniPixels passa a apontar para a célula(INSERÇÃO NO INÍCIO)*/
		else{
			nova->proxPixel = regiao->iniPixels;
			regiao->iniPixels = nova;
		}
		//Incrementa a quantidade de pixels da região
		regiao->nPixels += 1;
		//O pixel adicionado recebe o endereço da região a qual pertence
		img->pixel[lin][col].regiao = regiao;

		/*Se a região não for de borda faz a chamada recursiva para os quatro vizinhos do pixel */
		if(regiao->borda == FALSE){
			for (int i=0; i< 4; i++) {
				//Checa se o pixel analisado faz parte da imagem
				if(col+vetorPosSemBord[i] >= 0 && lin+vetorPosSemBord[(i+2)%4] >= 0 && col+vetorPosSemBord[i] < img->width && lin+vetorPosSemBord[(i+2)%4] < img->height ){
					regiao->nPixels = pixelsRegiao(img, limiar, col+vetorPosSemBord[i], lin+vetorPosSemBord[(i+2)%4], regiao);
				}
			}
		}
		/*Se a região for de borda, analisa os 8 vizinhos dele*/
		else{
			for (int i=0; i< 9; i++) {
				//A condição garante que o pixel analisado faz parte da imagem
				if(col+vetorPosicao[i%3] >= 0 && lin+vetorPosicao[i/3] >= 0 && col+vetorPosicao[i%3] < img->width && lin+vetorPosicao[i/3] < img->height ){
					regiao->nPixels = pixelsRegiao(img, limiar, col+vetorPosicao[i%3], lin+vetorPosicao[i/3], regiao);
				}
			}
		}
      return regiao->nPixels;
   }
   else{
      /*Caso o pixel ja tenha região ou não seja do mesmo tipo de borda da região, retorna
		a quantidade atual de pixels na região*/
      return regiao->nPixels;
   }
}
/* 
   \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
   IMPLEMENTACAO DAS FUNCOES DADAS

   ////////////////////////////////////////////////////////////////////// 
*/
/*-------------------------------------------------------------*/ 

/*-------------------------------------------------------------  
   mallocSafe 
   
   O parâmetro de mallocSafe é do tipo size_t.  
   Em muitos computadores, size_t é equivalente a unsigned int.  
   A função mallocSafe não está em nenhuma biblioteca e é desconhecida 
   fora destas notas de aula. 
   Ela é apenas uma abreviatura conveniente.

   Fonte:  http://www.ime.usp.br/~pf/algoritmos/aulas/aloca.html
*/
static void* mallocSafe(size_t nbytes){
	void *ptr;

	ptr = malloc (nbytes);
	if (ptr == NULL) 
	{
		fprintf (stderr, "Socorro! malloc devolveu NULL!\n");
		exit (EXIT_FAILURE_MALLOC);
	}
	return ptr;
}
/*-------------------------------------------------------------  
   luminosidadePixel

   Recebe um  ponteiro IMG para uma estrutura que representa 
   uma imagem, e a posicao [H][W] de um pixe da imagem
   e retorna a sua luminosidade de acordo com a formula

   luminosidade: (0.21 * r) + (0.72 * g) + (0.07 * b)

   http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/ 
   http://en.wikipedia.org/wiki/Relative_luminance

   Y = 0.2126 R + 0.7152 G + 0.0722 B

   Nota: há outras formulas para luminosidade por ai... 
*/
static double luminosidadePixel(Imagem *img, int col, int lin){
   return  (0.21 * img->pixel[lin][col].cor[RED] + 0.72 * img->pixel[lin][col].cor[GREEN] 
         	+ 0.07 * img->pixel[lin][col].cor[BLUE]);
}



