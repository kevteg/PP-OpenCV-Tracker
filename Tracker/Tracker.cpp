//Tracker
#include <opencv/cvaux.h>
#include <opencv/highgui.h> // Permite abrir las ventanas
#include <opencv/cxcore.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
//Kevin Hernández
int main (int argc, char*argv[]){

	CvSize CvSize640x480 = cvSize(640, 480); // Abrira todas las ventanas en 640 por 480. Al igual que la imagen de la camara
	CvCapture* p_captureWebCam;  // El vídeo de la cámara será asignado a este puntero
	IplImage* p_imgOriginal; //Puntero a la estructura de una imagen, sera la imagen de entrada de la cámara
	IplImage* p_imgProcessed; //Puntero a la estructura de una imagen, sera la imagen procesada
	//Ipl, Intel Image processing library, estandar usado por opencv 1.0 
	CvMemStorage* p_strStorage;  // variable necesaria de almacenaminto para luego convertir a	cvHoughCircles
	CvSeq* p_seqCircles; //Puntero a secuencia OpenCv, será retornado por cvHoughCircles y contendrá todos los circulos
						 // Se llama a cvGetSeqElem( p_seqCircles, 1); retornará un areglo del 1 circulo
	float* p_fltxyRadious; /*Arreglo que contendrá las coodernadas x [0], y [1] y el radio [3]*/
	int i; //Contador de bucles
	char charcheckforkeypress;//se verifica cuando se presiona una tecla para finalizar el ciclo
	p_captureWebCam = cvCaptureFromCAM(0); //se toma la imagen de la camara por defecto, si se tiene mas camaras se pueden tomar cambiando el indice que se envia
	if(!p_captureWebCam){ //Captura de la cámara fallo
		cout << "Error, la captura de la camara es nula" << endl;
		getchar();
		return(-1);
	}
	//Se crean las ventanas para abrir la imagen original y la procesada
	cvNamedWindow("Imagen original",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Imagen procesada", CV_WINDOW_AUTOSIZE);

	p_imgProcessed = cvCreateImage(CvSize640x480, // El objeto CvSize creado antes para que la imagen tenga siempre 640 x 480 pixeles
								   IPL_DEPTH_8U, // Profundidad del color de 8 bits
									1); // 1 canal, escala de grises, 
	while(1){ //Para cada frame
		p_imgOriginal = cvQueryFrame(p_captureWebCam);
		if(!p_imgOriginal){ //Ha fallado la captura del marco (frame) actual 	
			cout << "Error, el marco es nulo" << endl;
			getchar();
			break;
		}
		cvInRangeS(p_imgOriginal, // parametros de la función, imagen original, que se procesara 
			       CV_RGB(0, 100 , 0 ), // Valor del filtro minimo, si el color es mayor a este paramétro
				   CV_RGB(85,250,85),	// Valor del filtro máximo, si el color es menor a este paramétro
				   p_imgProcessed);  //Imagen procesada, copiara los filtros que se le hacen a la imagen original y al estar en escala de grises se mostrara de esa forma
	                   //En este punto debido a que la imagen procesada esta en escala de grises todo lo que se desea seguir estara de color blanco y el resto negro


		p_strStorage = cvCreateMemStorage(0); //Se pide la memoria necesaria para después convertir a cvHoughCircles)(
	             // se suaviza la imagen procesada para que sea mas fácil para la próxima función eligir los circulos
		cvSmooth(p_imgProcessed, //Input de la función
			     p_imgProcessed, //Output de la función
				 CV_GAUSSIAN, //se usa el filtro gauss (da tonalidad parecida a los pixeles cercanos, considerando mas a los pixeles mas cercanos)
				 9, //ancho de las secciones a suavizar 
				 9); //largo de las secciones a suavizar
		//Se llena la estructura con todos los circulos de la imagen procesada
		p_seqCircles = cvHoughCircles(p_imgProcessed, //Imagen input, debe estar en escala de grises
									  p_strStorage, // Da a la función memoria. Además hace que la función retorne puntero a CvSeq
									  CV_HOUGH_GRADIENT, // Única opción disponible para el algoritmo de detección de circulos
						              2, //
									  p_imgProcessed->height / 4, // distancia minima entre los centros de los circulos detectados (en pixeles)
								      100, //Mayor umbral en los bordes de los circulos detectados 		
									  50, //Menos umbral en los bordes de los circulos detectados
									  10, //Menor radio en pixeles
									  400); //Mayor radio en pixeles
		
		for(i = 0; i < p_seqCircles->total; i++){//Para cada circulo que haya sido detectado
			p_fltxyRadious = (float*)cvGetSeqElem(p_seqCircles, i); // Se obtienen las coordenadas del iésimo circulo
			cout << "Coordenadas de la pelota "<<i+1<<": x = " << p_fltxyRadious[0] << ", y = " << p_fltxyRadious[1] << ", radio = " << p_fltxyRadious[2]<< " :)" << endl;
			
			//Se dibuja un circulito pequeño dentro
			
			cvCircle(p_imgOriginal,//Se dibuja en la imagen original 
				cvPoint(cvRound(p_fltxyRadious[0]), cvRound(p_fltxyRadious[1])),//Centro del circulo
				3, // Radio del circulo
				CV_RGB(0,255,0), //Se dibuja color verde puro
				CV_FILLED); // Espesor
			//Dibujar un circulo azul grande
				cvCircle(p_imgOriginal,//Se dibuja en la imagen original 
				cvPoint(cvRound(p_fltxyRadious[0]), cvRound(p_fltxyRadious[1])),//Centro del circulo
				cvRound(p_fltxyRadious[2]), // Radio del circulo
				CV_RGB(0,0,250), //Se dibuja color azul puro
				3); // Espesor de tres pixeles
					 
		}//Fin For
		cvShowImage("Imagen original", p_imgOriginal); //Se muestra la imagen original
		cvShowImage("Imagen procesada", p_imgProcessed); //Se muestra la imagen procesada
		cvReleaseMemStorage(&p_strStorage); // se libera la memoria de los circulos actuales
		charcheckforkeypress = cvWaitKey(20); //delay en ms, y se obtiene la letra que se presione
		if(charcheckforkeypress == 27) // Si se ha presionado esc (ASCI 27) se rompe el ciclo infinito 
	      break;
	}//Fin While
	cvReleaseCapture(&p_captureWebCam); // liberar memoria
	cvDestroyWindow("Imagen original");
	cvDestroyWindow("Imagen procesada");
	return(0);
}
