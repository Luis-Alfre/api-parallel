#include <mpi.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>

#include <omp.h>
#include "./utils.c"

void isla(int poblacion[], int pobAux[], double puntAcumulada[],
		  double aptitud[], double distancias[],
		  double nuevaAptitud[], double nuevaDistancias[],
		  double matrizDistancias[], double puntuacion[],
		  double probMut, double probCruce,
		  int tamPoblacion, int nCiudades)
{
	seleccion(poblacion, puntAcumulada, aptitud, distancias, // datos entrada
			  pobAux, nuevaAptitud, nuevaDistancias,		 // Salida de la funcion
			  tamPoblacion, nCiudades);						 // datos entrada
	extraerMatriz(pobAux, poblacion, tamPoblacion, nCiudades);
	extraerVectorDouble(nuevaDistancias, distancias, tamPoblacion);
	extraerVectorDouble(nuevaAptitud, aptitud, tamPoblacion);
	// printf("\nHilo: %d\n", hilo);
	reproduccion(poblacion, distancias, aptitud, probCruce,
				 matrizDistancias, tamPoblacion, nCiudades);

	mutacion(poblacion, aptitud, matrizDistancias, distancias, tamPoblacion, probMut, nCiudades);
	int posMejor = evaluacion(aptitud, puntuacion, puntAcumulada, tamPoblacion); // datos entrada
}

void intercambioMejor(int *poblacion, int *mejores, int *peores,
					  int tamPoblacion, int nCiudades, int nIntercambios)
{
	for (int i = 0; i < nIntercambios; i++)
	{
		int *individuo = (int *)malloc(nCiudades * sizeof(int));
		extraerVector(poblacion, individuo, mejores[i], tamPoblacion, nCiudades);
		insertarVector(poblacion, individuo, peores[i], tamPoblacion, nCiudades);
	}
}

void intercambioEnAnillo(int *poblacionPrincipal, int *mejorCamino,
						 int *mejorCaminoAdyacente, int *peores,
						 int rank, MPI_Status estado,
						 int size, int tamPoblacion, int nCiudades, int nIntercambios)
{
	if (rank == 0)
	{
		MPI_Send(mejorCamino,				// variable con la referencia dell vector de elementos a enviar
				 nCiudades * nIntercambios, // tamaño del vector que se envia
				 MPI_INT,					// Tipo de dato que se envia
				 rank + 1,					// identificador del proceso destino
				 0,							// etiqueta informativa
				 MPI_COMM_WORLD);			// Comunicador utilizado

		MPI_Recv(mejorCaminoAdyacente,		// variable con la referencia al vector para almacenar el dato recibido
				 nCiudades * nIntercambios, // tamaño del vector que se recibe
				 MPI_INT,					// Tipo de dato que se recibe
				 size - 1,					// identificador del proceso del que se reciben los datos
				 0,							// etiqueta informativa
				 MPI_COMM_WORLD,			// Comunicador utilizados
				 &estado);					// estructura con información del estado

		// intercambio
		intercambioMejor(poblacionPrincipal, mejorCaminoAdyacente, peores,
						 tamPoblacion, nCiudades, nIntercambios);
	}
	else
	{
		MPI_Recv(mejorCaminoAdyacente,		// variable con la referencia al vector para almacenar el dato recibido
				 nCiudades * nIntercambios, // tamaño del vector que se recibe
				 MPI_INT,					// Tipo de dato que se recibe
				 rank - 1,					// identificador del proceso del que se reciben los datos
				 0,							// etiqueta informativa
				 MPI_COMM_WORLD,			// Comunicador utilizados
				 &estado);					// estructura con información del estado

		// intercambio
		intercambioMejor(poblacionPrincipal, mejorCaminoAdyacente, peores,
						 tamPoblacion, nCiudades, nIntercambios);

		if (rank != size - 1)
		{
			MPI_Send(mejorCamino,
					 nCiudades * nIntercambios,
					 MPI_INT,
					 rank + 1,
					 0,
					 MPI_COMM_WORLD);
		}
		else
		{
			MPI_Send(mejorCamino,
					 nCiudades * nIntercambios,
					 MPI_INT,
					 0,
					 0,
					 MPI_COMM_WORLD);
		}
	}
}

int main(int argc, char *argv[])
{
	// DATOS MPI
	int rank, size, *enviado, *recibido;

	MPI_Status estado;
	// Se inicia el entorno de comunicación MPI
	MPI_Init(&argc, &argv);
	// Se obtiene el total de procesos
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Se obtiene el identificador de cada proceso
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// DATOS INICIALES
	srand(time(NULL));

	int nCiudades;			// nodos del grafo
	int tamPoblacion; // tama�o de la poblaci�n
	int nMaxGen;
	int pasosIntercambio = 2;
	int nIntercambios = 10;

	int posMejor = 0; // posici�n del individuo mejor adaptado
	double probCruce;
	double probMut;

	char parameters [] = "parameters.txt";
	FILE *f;
	f = fopen (parameters, "r");//se abre el archivo
	fscanf(f,"%d", &nCiudades);
	fscanf(f,"%d", &tamPoblacion);
	fscanf(f,"%lf", &probCruce);
	fscanf(f,"%lf", &probMut);
	fscanf(f,"%d", &nMaxGen);
	fclose(f);

	
	int *coordenadasX = (int *)malloc(nCiudades * sizeof(int));
	int *coordenadasY = (int *)malloc(nCiudades * sizeof(int));

	int *poblacionPrincipal = (int *)malloc(tamPoblacion * nCiudades * sizeof(int));
	int *mejorCamino, *mejorCaminoAdyacente, *pobAuxiliar,
		*padre1, *padre2, *hijo1, *hijo2, *intIndice;
	double *matrizDistancias, *distancias, *aptitud, *puntuacion,
		*puntAcumulada, *nuevaAptitud, *nuevaDistancias;
	double longitud;
	intIndice = ( int * ) malloc ( nCiudades * sizeof ( int ) );
	mejorCamino = (int *)malloc(nCiudades * sizeof(int));
	mejorCaminoAdyacente = (int *)malloc(nCiudades * sizeof(int));
	distancias = (double *)malloc(tamPoblacion * sizeof(double));
	nuevaDistancias = (double *)malloc(tamPoblacion * sizeof(double));
	aptitud = (double *)malloc(tamPoblacion * sizeof(double));
	nuevaAptitud = (double *)malloc(tamPoblacion * sizeof(double));
	puntuacion = (double *)malloc(tamPoblacion * sizeof(double));
	puntAcumulada = (double *)malloc(tamPoblacion * sizeof(double));
	matrizDistancias = (double *)malloc(nCiudades * nCiudades * sizeof(double));
	pobAuxiliar = (int *)malloc(tamPoblacion * nCiudades * sizeof(int));

	char nombreArchivoX[] = "coordenadasX-MPI.txt";
	char nombreArchivoIncialX[] = "coordenadasX-incial.txt";
	leerValorEnteroArchivo(nombreArchivoX,coordenadasX, nCiudades);
	imprimirVectorEnteroFichero(coordenadasX, nCiudades, nombreArchivoIncialX);
	char nombreArchivoY[] = "coordenadasY-MPI.txt";
	char nombreArchivoIncialY[] = "coordenadasY-incial.txt";
	leerValorEnteroArchivo(nombreArchivoY,coordenadasY, nCiudades);
	imprimirVectorEnteroFichero(coordenadasY, nCiudades, nombreArchivoIncialY);


	crearMatrizDistancia(matrizDistancias, coordenadasX, coordenadasY, nCiudades);
	poblacionInicial(poblacionPrincipal, distancias, aptitud, tamPoblacion, nCiudades, matrizDistancias);

	posMejor = evaluacion(aptitud, puntuacion, puntAcumulada, tamPoblacion);
	extraerVector(poblacionPrincipal, mejorCamino, posMejor, tamPoblacion, nCiudades);
	char nombreCamino[] = "caminoInicial-MPI.txt";
	imprimirVectorEnteroFichero(mejorCamino, nCiudades, nombreCamino);
	longitud = longitudCircuito(mejorCamino, matrizDistancias, nCiudades);
	double longitudes[2];

	longitudes[0]=longitud;
	for (int i = 0; i < nIntercambios; i++)
	{
		if (i % pasosIntercambio == 0)
		{
			int *mejores = (int *)malloc(nIntercambios * sizeof(int));
			obtenerMejores(aptitud, tamPoblacion, mejores, nIntercambios);

			int *peores = (int *)malloc(nIntercambios * sizeof(int));
			obtenerMejores(aptitud, tamPoblacion, peores, nIntercambios);

			intercambioEnAnillo(poblacionPrincipal,
								mejores, mejorCaminoAdyacente, peores,
								rank, estado, size, tamPoblacion,
								nCiudades, nIntercambios);
			recalcularAptitud(poblacionPrincipal, aptitud, matrizDistancias,
							  distancias, tamPoblacion, nCiudades);
		}

		isla(poblacionPrincipal, pobAuxiliar, puntAcumulada,
			 aptitud, distancias,
			 nuevaAptitud, nuevaDistancias,
			 matrizDistancias, puntuacion,
			 probMut, probCruce,
			 tamPoblacion, nCiudades);
	}

	posMejor = evaluacion(aptitud, puntuacion, puntAcumulada, tamPoblacion); // datos entrada
	// double time_2 = omp_get_wtime();
	printf("Mejor distancia : %d \t\t %.2f \n", posMejor, distancias[posMejor]);
	// imprimirVectorReales(aptitud, nCiudades);
	extraerVector(poblacionPrincipal, mejorCamino, posMejor, tamPoblacion, nCiudades);
	char nombreCaminoFinal[] = "caminoFinal-MPI.txt";
	imprimirVectorEnteroFichero(mejorCamino, nCiudades, nombreCaminoFinal);
	char texLongitudes[] = "longitudesIsla-MPI.txt";
	
	longitudes[1]=distancias[posMejor];
	imprimirVectorRealFichero(longitudes,2,texLongitudes);

	MPI_Finalize();
}
