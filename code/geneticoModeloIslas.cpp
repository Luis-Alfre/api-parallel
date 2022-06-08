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


	printf("--------------\n\n");
	imprimirVectorEnteros(pos1, nCiudades);
	printf("--------------");

}

void genetico(int ciudades, int tamañoPoblacion, double proMutacion, double proCruce, int numMaxGen)
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
	double longitud;
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
	char  *nombreArchivo = (char*)"bays29.tsp";
	//nCiudades = leerArchivo( nombreArchivo, intIndice, coordenadasX, coordenadasY);
	generarCoordenadas(coordenadasX, coordenadasY, nCiudades, xMin, xMax, yMin, yMax);  
	crearMatrizDistancia(matrizDistancias, coordenadasX, coordenadasY, nCiudades);
	
	
	// Generaci�n de poblaci�n inicial para cada isla
	poblacionInicial(poblacion1,distancias1,aptitud1,tamPoblacion,nCiudades,matrizDistancias);
	poblacionInicial(poblacion2,distancias2,aptitud2,tamPoblacion,nCiudades,matrizDistancias);
	//poblacionInicial(poblacion3,distancias3,aptitud3,tamPoblacion,nCiudades,matrizDistancias);
	//poblacionInicial(poblacion4,distancias4,aptitud4,tamPoblacion,nCiudades,matrizDistancias);
	
	//printf("Poblaciones iniciales listas : \n");
	
	
	
	int ciclos = numMaxGen;
	int posMejor1 = 0, posMejor2 = 0, posMejor3 = 0, posMejor4 = 0; // posici�n del individuo mejor adaptado
		
	for(int i=0;i<ciclos;i++)//migraciones
	{
		#pragma omp parallel num_threads(2)
		#pragma omp sections
		{
			# pragma omp section
			{
				int *indi = ( int * ) malloc ( nCiudades * sizeof ( int ) );
				extraerVector(poblacion1,indi, 0, tamPoblacion, nCiudades);
				printf("\n Antes: ");
				imprimirVectorEnteros(indi,nCiudades);
				posMejor1 = funcionGenetico(poblacion1, matrizDistancias,distancias1,aptitud1, tamPoblacion, nCiudades);
				
				extraerVector(poblacion1,indi, 0, tamPoblacion, nCiudades);
				printf("\n Despues: ");
				imprimirVectorEnteros(indi,nCiudades);
			}
			# pragma omp section
			{
				posMejor2 = funcionGenetico(poblacion2, matrizDistancias,distancias2,aptitud2, tamPoblacion, nCiudades);
			}
			
		}
		// Aqui deber�a incluir el intercambio de elementos
		if(i%10==0) {
			//mejor posicion de cada isla, isla origen, isla destino
			intercambio(posMejor1, posMejor2, poblacion1, poblacion2, nCiudades, tamPoblacion);
			
		}

	}
	
}


extern "C"
{
    extern void cffi_suma_enteros(int ciudades, int tamanioPoblacion, double proMutacion, double proCruce, int numMaxGen)
    {
        return genetico(ciudades,tamanioPoblacion,proMutacion,proCruce,numMaxGen);
    }
}


