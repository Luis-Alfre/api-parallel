#include<stdio.h>
#include<time.h>
#include<math.h>
#include<omp.h>
#include <sys/stat.h>
#include <cstring>
#include "utils.cpp"


void intercambio(int posMejor1, int posMejor2, int isla1[], int isla2[], int nCiudades, int tamañoPoblacion){
	
	int *pos1, *pos2;
	pos1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	pos2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );

	printf("************");
	imprimirVectorEnteros(isla1, nCiudades);
	printf("************");

	extraerVector(isla1,pos1,posMejor1,tamañoPoblacion, nCiudades);
	extraerVector(isla2,pos2,posMejor2,tamañoPoblacion, nCiudades);
	insertarVector(isla1,pos2, posMejor1,tamañoPoblacion, nCiudades);
	insertarVector(isla2,pos1, posMejor2,tamañoPoblacion, nCiudades);

}

void genetico(int ciudades, 
			int tamañoPoblacion, 
			double proMutacion, 
			double proCruce, 
			int numMaxGen, 
			int x[], 
			int y[])
{
	// Para inlcuir caracteres espa�o�
	//setlocale(LC_ALL, "spanish"); // en Linux
	// Semilla para n�meros aleatorios
	srand(time(NULL)); 
    
    // Declaracion variables - par�metros del algoritmo
    
	// int nCiudades = 29; // nodos del grafo
	// int tamPoblacion = 300; // tama�o de la poblaci�n
	int nCiudades = ciudades;
	int tamPoblacion = tamañoPoblacion;
	int mutados;
	int xMin = 1, xMax = 20, yMin = 1, yMax = 20; // limites de las coordenadas del grafo

	
	// Variables dinamicas para almacenamiento de vectores y matrices
	int *coordenadasX, *coordenadasY, *intIndice,
	    *poblacion1, *poblacion2, *poblacion3, *poblacion4, 
		*mejorCamino, *pobAuxiliar;
	    //*padre1, *padre2, *hijo1, *hijo2;
	double  *matrizDistancias, *distancias1, *distancias2, *distancias3, *distancias4, *puntuacion, 
	        *puntAcumulada, *nuevaAptitud, *nuevaDistancias,
	        *aptitud1,*aptitud2, *aptitud3, *aptitud4;  
	double longitud1;
	double longitud2;
	double longitud3;
	double longitud4;

	double longitudes[2];
	//double probCruce = 0.8;
	//double probMutacion = 0.1;
	double probCruce = proCruce;
	double probMutacion = proMutacion;
	// reserva de memoria para variables din�micas	
	//padre1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	//padre2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	//hijo1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	//hijo2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	
	intIndice = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	coordenadasX = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	coordenadasY = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	mejorCamino = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	distancias1 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	distancias2 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	distancias3 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	distancias4 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	
	
	matrizDistancias = ( double * ) malloc ( nCiudades * nCiudades * sizeof ( double ) );
	
	poblacion1 = ( int * ) malloc ( tamPoblacion * nCiudades * sizeof ( int ) );
	poblacion2 = ( int * ) malloc ( tamPoblacion * nCiudades * sizeof ( int ) );
	poblacion3 = ( int * ) malloc ( tamPoblacion * nCiudades * sizeof ( int ) );
	poblacion4 = ( int * ) malloc ( tamPoblacion * nCiudades * sizeof ( int ) );
	
	aptitud1 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	aptitud2 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	aptitud3 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	aptitud4 = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
		
	printf (" \n ======Fundamentos de computaci�n paralela y distribuida ======");
	printf (" \n       Algoritmo Gen�tico Simple para el problema del TSP");
	printf (" \n                 Versi�n serial y paralela \n");
	printf (" \n N�mero de ciudades: %d", nCiudades);
	printf (" \n Tama�o de la poblaci�n: %d",tamPoblacion);
	printf (" \n Numero de generaciones: %d", numMaxGen);


	// Generacion del problema
	coordenadasX = x;
	coordenadasY = y;
	// Se imprime el vector en un archivo de texto.
	char cX[]= "coordenadasX.txt";
	imprimirVectorEnteroFichero(x,nCiudades,cX);
	char cY[] = "coordenadasY.txt";
	imprimirVectorEnteroFichero(y,nCiudades,cY);
	//nCiudades = leerArchivo( nombreArchivo, intIndice, coordenadasX, coordenadasY);
	//generarCoordenadas(coordenadasX, coordenadasY, nCiudades, xMin, xMax, yMin, yMax);  
	crearMatrizDistancia(matrizDistancias, coordenadasX, coordenadasY, nCiudades);
	
	
	// Generaci�n de poblaci�n inicial para cada isla
	poblacionInicial(poblacion1,distancias1,aptitud1,tamPoblacion,nCiudades,matrizDistancias);
	poblacionInicial(poblacion2,distancias2,aptitud2,tamPoblacion,nCiudades,matrizDistancias);
	poblacionInicial(poblacion3,distancias3,aptitud3,tamPoblacion,nCiudades,matrizDistancias);
	poblacionInicial(poblacion4,distancias4,aptitud4,tamPoblacion,nCiudades,matrizDistancias);
	
	//printf("Poblaciones iniciales listas : \n");
	int posMejor1=0, posMejor2=0, posMejor3=0, posMejor4=0;
	puntuacion = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	puntAcumulada = ( double * ) malloc ( tamPoblacion * sizeof ( double ) );
	
	// posMejor1 = evaluacion(aptitud1, puntuacion, puntAcumulada, tamPoblacion);
	// posMejor2 = evaluacion(aptitud2, puntuacion, puntAcumulada, tamPoblacion);
	// posMejor3 = evaluacion(aptitud3, puntuacion, puntAcumulada, tamPoblacion);
	// posMejor4 = evaluacion(aptitud4, puntuacion, puntAcumulada, tamPoblacion);
	
	int *mejorCamino1, *mejorCamino2, *mejorCamino3,*mejorCamino4;

	mejorCamino1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	mejorCamino2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	mejorCamino3 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	mejorCamino4 = ( int * ) malloc ( nCiudades * sizeof ( int ) );

	extraerVector(poblacion1,mejorCamino1,0,tamPoblacion, nCiudades);
	extraerVector(poblacion2,mejorCamino2,0,tamPoblacion, nCiudades);
	extraerVector(poblacion3,mejorCamino3,0,tamPoblacion, nCiudades);
	extraerVector(poblacion4,mejorCamino4,0,tamPoblacion, nCiudades);
	
	longitud1 = longitudCircuito(mejorCamino1, matrizDistancias,nCiudades);
	longitud2 = longitudCircuito(mejorCamino2, matrizDistancias,nCiudades);
	longitud3 = longitudCircuito(mejorCamino3, matrizDistancias,nCiudades);
	longitud4 = longitudCircuito(mejorCamino4, matrizDistancias,nCiudades);
	
	
	double longitudes1[2];
	double longitudes2[2];
	double longitudes3[2];
	double longitudes4[2];
	
	longitudes1[0]= longitud1;
	longitudes2[0]= longitud2;
	longitudes3[0]= longitud3;
	longitudes4[0]= longitud4;
	
	char caminoInicialIsla1[] = "caminoInicialIsla1.txt";
	char caminoInicialIsla2[] = "caminoInicialIsla2.txt";
	char caminoInicialIsla3[] = "caminoInicialIsla3.txt";
	char caminoInicialIsla4[] = "caminoInicialIsla4.txt";
	
	imprimirVectorEnteroFichero(mejorCamino1,nCiudades,caminoInicialIsla1);
	imprimirVectorEnteroFichero(mejorCamino2,nCiudades,caminoInicialIsla2);
	imprimirVectorEnteroFichero(mejorCamino3,nCiudades,caminoInicialIsla3);
	imprimirVectorEnteroFichero(mejorCamino4,nCiudades,caminoInicialIsla4);
	
	int *MejorRuta1, *MejorRuta2, *MejorRuta3, *MejorRuta4;

	MejorRuta1 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	MejorRuta2 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	MejorRuta3 = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	MejorRuta4 = ( int * ) malloc ( nCiudades * sizeof ( int ) );

	double mRta1 = longitud1, mRta2 = longitud2, mRta3 = longitud3, mRta4 = longitud4;



	int ciclos = numMaxGen;
		
	for(int i=0;i<ciclos;i++)//migraciones
	{
		// Aqui deber�a incluir el intercambio de elementos
		if(i%10==0) {
			//mejor posicion de cada isla, isla origen, isla destino
			intercambio(posMejor1, posMejor2, poblacion1, poblacion2, nCiudades, tamPoblacion);
			intercambio(posMejor2, posMejor3, poblacion2, poblacion3, nCiudades, tamPoblacion);
			intercambio(posMejor3, posMejor4, poblacion3, poblacion4, nCiudades, tamPoblacion);
			intercambio(posMejor4, posMejor1, poblacion4, poblacion1, nCiudades, tamPoblacion);
		}

		#pragma omp parallel num_threads(2)
		#pragma omp sections
		{
			# pragma omp section
			{
				//int *indi = ( int * ) malloc ( nCiudades * sizeof ( int ) );
				//extraerVector(poblacion1,indi, 0, tamPoblacion, nCiudades);
				//printf("\n Antes: ");
				//imprimirVectorEnteros(indi,nCiudades);
				posMejor1 = funcionGenetico(poblacion1, matrizDistancias,distancias1,aptitud1, tamPoblacion, nCiudades, probCruce, probMutacion);
				extraerVector(poblacion1,mejorCamino1,posMejor1,tamPoblacion, nCiudades);
				longitud1 = longitudCircuito(mejorCamino1, matrizDistancias,nCiudades);
				if (longitud1<mRta1){
					extraerVector(poblacion1,MejorRuta1,posMejor1,tamPoblacion, nCiudades);
					mRta1 = longitud1;
				}

			}
			# pragma omp section
			{
				posMejor2 = funcionGenetico(poblacion2, matrizDistancias,distancias2,aptitud2, tamPoblacion, nCiudades, probCruce, probMutacion);
				extraerVector(poblacion2,mejorCamino2,posMejor2,tamPoblacion, nCiudades);
				longitud2 = longitudCircuito(mejorCamino2, matrizDistancias,nCiudades);
				if (longitud2<mRta2){
					extraerVector(poblacion2,MejorRuta2,posMejor2,tamPoblacion, nCiudades);
					mRta2 = longitud2;
				}
			}
			# pragma omp section
			{
				posMejor3 = funcionGenetico(poblacion3, matrizDistancias,distancias3,aptitud3, tamPoblacion, nCiudades, probCruce, probMutacion);
				extraerVector(poblacion1,mejorCamino3,posMejor3,tamPoblacion, nCiudades);
				longitud3 = longitudCircuito(mejorCamino3, matrizDistancias,nCiudades);
				if (longitud3<mRta3){
					extraerVector(poblacion3,MejorRuta3,posMejor3,tamPoblacion, nCiudades);
					mRta3 = longitud3;
				}
			}
			# pragma omp section
			{
				posMejor4 = funcionGenetico(poblacion4, matrizDistancias,distancias4,aptitud4, tamPoblacion, nCiudades, probCruce, probMutacion);
				extraerVector(poblacion4,mejorCamino4,posMejor4,tamPoblacion, nCiudades);
				longitud4 = longitudCircuito(mejorCamino4, matrizDistancias,nCiudades);
				if (longitud4<mRta4){
					extraerVector(poblacion4,MejorRuta4,posMejor4,tamPoblacion, nCiudades);
					mRta4 = longitud4;
				}
			}
			
		}
		

	}
		printf("\n Longitud del camino final 1: %.2f \n",mRta1 );
		printf("\n Longitud del camino final 2: %.2f \n",mRta2 );
		printf("\n Longitud del camino final 3: %.2f \n",mRta3 );
		printf("\n Longitud del camino final 4: %.2f \n",mRta4 );
		double mejorLong = mRta1;
	if(mejorLong>mRta2) {
		mejorLong=mRta2;
	}
	if(mejorLong>mRta3) {
		mejorLong=mRta3;
	}
	if(mejorLong>mRta4) {
		mejorLong=mRta4;
	}
	if(mejorLong==mRta1) {
		mejorLong = mRta1;
	}	
		printf("\n Longitud del camino al final(mejor longitud) : %.2f \n",mejorLong);

	char caminoFinalIsla1[] = "caminoFinalIsla1.txt";
	char caminoFinalIsla2[] = "caminoFinalIsla2.txt";
	char caminoFinalIsla3[] = "caminoFinalIsla3.txt";
	char caminoFinalIsla4[] = "caminoFinalIsla4.txt";
	
	imprimirVectorEnteroFichero(MejorRuta1,nCiudades,caminoFinalIsla1);
	imprimirVectorEnteroFichero(MejorRuta2,nCiudades,caminoFinalIsla2);
	imprimirVectorEnteroFichero(MejorRuta3,nCiudades,caminoFinalIsla3);
	imprimirVectorEnteroFichero(MejorRuta4,nCiudades,caminoFinalIsla4);
	
	char texLongitudesIsla1[] = "longitudesIsla1.txt";
	char texLongitudesIsla2[] = "longitudesIsla2.txt";
	char texLongitudesIsla3[] = "longitudesIsla3.txt";
	char texLongitudesIsla4[] = "longitudesIsla4.txt";
	
	longitudes1[1]= mRta1;	
	longitudes2[1]= mRta2;	
	longitudes3[1]= mRta3;	
	longitudes4[1]= mRta4;	
	
	imprimirVectorRealFichero(longitudes1,2,texLongitudesIsla1);
	imprimirVectorRealFichero(longitudes2,2,texLongitudesIsla2);
	imprimirVectorRealFichero(longitudes3,2,texLongitudesIsla3);
	imprimirVectorRealFichero(longitudes4,2,texLongitudesIsla4);
	
}


extern "C"
{
    extern void cffi_suma_enteros(int ciudades, 
									int tamanioPoblacion, 
									double proMutacion, 
									double proCruce, 
									int numMaxGen,
									int x[], 
									int y[])
    {
        return genetico(ciudades,
						tamanioPoblacion,
						proMutacion,
						proCruce,
						numMaxGen,
						x,
						y);
    }
}


