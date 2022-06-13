import cffi

ffi = cffi.FFI()
ffi.cdef("void cffi_suma_enteros(int ciudades, int tamanioPoblacion, double proMutacion, double proCruce, int numMaxGen, int x[], int y[]);")
C = ffi.dlopen("/home/luis/Documentos/Projects/Api-Parallel/code/islas.so")
def suma_enteros(ciudades,tamanioPoblacion,proMutacion,proCruce,numMaxGen,x,y):
    return C.cffi_suma_enteros(ciudades,tamanioPoblacion,proMutacion,proCruce,numMaxGen,x,y)



