/**
* Reconhecimento de Padrões // Aprendizagem de Máquina
* Alessandro L. Koerich
* alekoe@computer.org
* 2009
*
* EXEMPLO 1: Manipulando imagens coloridas RGB
*
**/

// INCLUDES
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>

// DEFINES
#define LIMIAR 127
#define MAX_VALUE 255
#define NUM_EXEMPLOS 80

// Bart Train: 80 items: bart1.bmp - bart80.bmp
// Homer Train 62 items: homer1.bmp - homer62.bmp
// Bart Test: 54 items: bart116.bmp - bart169.bmp
// Homer Test: 37 items: homer88.bmp - homer124.bmp

// MAIN
int main( int argc, char** argv )   
{

	// Vetor de características

	// Variável que armazenará a tecla pressionada
	int tecla;

	// Variáveis de laço
	int h, w, ii, jj, iNum;

	// Variáveis que armazenam o valor RGB do pixel
	unsigned char red, blue, green;

	// Vetor de características
	float fVetor[100][5];

	// Variavel Caracteristica Laranja
	float fLaranja, fBranca;

	// Nome do arquivo de imagem a ser aberto
	static char cArquivo[50] = {'\0'};

	FILE *fp;

	fp = fopen ("BartTraina.txt","a");

	if(fp == NULL) {
		perror("failed to open BartTreina.txt");
		return EXIT_FAILURE;
	}

	//Variável que armazenará a imagem.
	//A estrutura IplImage contém várias informações sobre a imagem (ver manual OpenCV).	
	IplImage *img = NULL;
	IplImage *processada = NULL;
	IplImage *threshold = NULL;
	
	//variável que guarda a largura e altura de uma imagem
	CvSize tam;

	//guarda o valor do pixel da imagem
	CvScalar elemento;
	
	// Inicializa fVetor
	for ( ii = 0 ; ii < 100 ; ii++ )
	{
		for ( jj = 0; jj < 5; jj++ )
		{
			fVetor[ii][jj] = 0.0;
		}
	}

	// Inicializa cArquivo
	for ( ii = 0 ; ii < 50 ; ii++ )
	{
		cArquivo[ii] = '\0';
	}

	cvNamedWindow( "Processada", CV_WINDOW_AUTOSIZE );

	// Pega todas as imagens do arquivo
	for ( iNum = 1; iNum <= NUM_EXEMPLOS; iNum++ )
	{
    
		int iNum2 = iNum;   //int iNum2 = iNum + 115; // 87
		// Forma o nome do arquivo de imagem a ler do disco
		sprintf ( cArquivo, "Train/bart%d.bmp", (int)(iNum2) ); 
		printf ( "%s\n", cArquivo);

		// Carrega a imagem em img.
		// 1  - carrega a imagem em 3 canais (colorida)
		// 0  - carrega a imagem em 1 canal  (níveis de cinza)
		// -1 - carrega a imagem como ela é  (depende do arquivo)
		//img = cvLoadImage("bart012.jpg", -1);
		img = cvLoadImage( cArquivo, -1 );

		// Armazena o tamanho (largura, altura) da imagem 'img' em tam
		tam = cvGetSize( img );

		// Cria o header da imagem e aloca memória (tam) para processada.
		// 1 - imagem em níveis de cinza
		// 3 - imagem colorida	
		// processada = cvCreateImage(tam, IPL_DEPTH_8U, 3);

		//faz um clone de img em processada e em threshold
		processada = cvCloneImage( img );
		threshold  = cvCloneImage( img );

		// Inicializa variável 
		fLaranja = 0.0;
		fBranca = 0.0;

		// Laço para acessar os pixels da imagem
		for( h = 0; h < img->height; h++ )//linhas
		{
			for( w = 0; w < img->width; w++ )//colunas
			{
				// Le os valores de cada um dos canais e escreve em blue, green, red. Note que
				// OpenCV usa a ordem BGR
				blue  = ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 0 ];
				green = ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 1 ];
				red   = ( (uchar *)(img->imageData + h*img->widthStep) )[ w*img->nChannels + 2 ];

				// Imprime o valor do pixel
				//printf( "pixel[%d][%d]=%d %d %d \n", h, w, (int)blue, (int)green, (int)red );

				// Verifica os pixels que tem valor laranja e pinta de verde em processada
				if ( blue>=11 && blue<=22 && green>=85 && green<=105 &&  red>=240 && red<=255 )
				{
					( (uchar *)(processada->imageData + h*processada->widthStep) )[ w*processada->nChannels + 0 ] = 0; 
					( (uchar *)(processada->imageData + h*processada->widthStep) )[ w*processada->nChannels + 1 ] = 255; 
					( (uchar *)(processada->imageData + h*processada->widthStep) )[ w*processada->nChannels + 2 ] = 0; 
				}

				// Verifica se os pixels que tem valor laranja e conta o número de pixels
				if ( blue>=11 && blue<=22 && green>=85 && green<=105 &&  red>=240 && red<=255 )
				{
					fLaranja++;
				}

				// Verifica se os pixels que tem cor branca e conta o número de pixels
				if ( blue >= 253 && green >= 253 && red >= 253 )
				{
					fBranca++;
				}
			}
		}

		// Normaliza a característica pelo número de pixel total da imagem
		fLaranja = fLaranja / ( (int)img->height * (int)img->width );
		fBranca  = fBranca  / ( (int)img->height * (int)img->width );

		// Escreve as características em um Vetor
		fVetor[iNum][1] = fLaranja;
		fVetor[iNum][2] = fBranca;

		printf( "\n%d %f %f", iNum, fVetor[iNum][1], fVetor[iNum][2] );

		fprintf( fp, "%f,", fVetor[iNum][1]);
		fprintf( fp, "%f,", fVetor[iNum][2]);
		fprintf( fp, "%s\n", "Bart");





		cvShowImage( "Original", img );
		cvShowImage( "Processada", processada );
		tecla = cvWaitKey(0);
	}

	// Outros Exemplos de Funções do OpenCV

	// Limiariza a imagem 'img' guardando o resultado na imagem 'threshold'	
	cvThreshold( img, threshold, LIMIAR, MAX_VALUE, CV_THRESH_BINARY );

	// Cria uma janela que exibe imagens.	
	cvNamedWindow( "Original", CV_WINDOW_AUTOSIZE );

	// Mostra a imagem 'img' na janela "Original" criada anteriormente.
	cvShowImage( "Original", img );

	cvNamedWindow( "Processada", CV_WINDOW_AUTOSIZE );
	cvShowImage( "Processada", processada );

	// a função cvWaitKey retorna o valor da tecla pressionada
	// 0  - Fica esperando um evento do teclado
    // >0 - Tempo de delay em milisegundos (exemplo: cvWaitKey(5000) = 5 segundos)

	// Função necessária para se poder visualizar as janelas na tela antes que elas desapareçam.
	tecla = cvWaitKey(0);
	
	//imprime a tecla pressionada
	printf("tecla: %c\n", (char)tecla);

	cvReleaseImage(&img);
	cvDestroyWindow("Original");

	cvReleaseImage(&processada);
	cvDestroyWindow("Processada");

	fclose(fp);

	return 0;
} 