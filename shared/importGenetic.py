import cffi
from ctypes import *


ffi = cffi.FFI()
ffi.cdef("void genetic(int ciudades, int tamanioPoblacion, double proMutacion, double proCruce, int numMaxGen, int x[], int y[]);")
C = ffi.dlopen("/home/luis/Documentos/Projects/Api-Parallel/api/app/code/geneticMP/islas.so")
def genetic(ciudades,tamanioPoblacion,proMutacion,proCruce,numMaxGen,x,y):
    return C.genetic(ciudades,tamanioPoblacion,proMutacion,proCruce,numMaxGen,x,y)

#Cargamos la librería
# adder = CDLL('/home/luis/Documentos/Projects/Api-Parallel/code/geneticMPI/islas.so')

#Realizamos la suma entera

# def geneticMPI():
#     adder.maino(1,0)
