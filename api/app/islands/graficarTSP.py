# -*- coding: utf-8 -*-
"""
Universidad de Pamplona
Facultad de Ingenierías y Arquitectura
Programa de Ingeniería de Sistemas
Profesor: José Orlando Maldonado Bautista

ALGORITMO GENETICO SIMPLE APLICADO AL TSP
"""
import numpy as np
import matplotlib.pyplot as plt
import os


def graficarCamino(coordenadas,camino, name):
    [nCiudades,nc] = coordenadas.shape  
    c = coordenadas
    r = camino
    plt.scatter(c[:,0],c[:,1])

    for i in range(nCiudades):
        if (i<nCiudades-1):
            j = i+1
        else:
            j=0
        p1 = np.array([c[r[i],0],c[r[j],0]])
        p2 = np.array([c[r[i],1],c[r[j],1]]) 
        plt.plot(p1,p2)
    #plt.show()
  
        
        
######################## CODIGO PRINCIPAL ########################


def graph(coordenadasX, coordenadasY, longitud, camInicial, camFinal, name):
    #os.system("geneticoSimple_06.exe")
    #os.system("geneticoSimple_serial_completar.exe")
    plt.subplots()
    fichero01 = coordenadasX
    raw_data = open(fichero01)
    cX = np.loadtxt(raw_data, delimiter=",")

    fichero02 = coordenadasY
    raw_data = open(fichero02)
    cY = np.loadtxt(raw_data, delimiter=",")

    fichero03 = longitud
    raw_data = open(fichero03)
    longitudes = np.loadtxt(raw_data, delimiter=",")

    print(cY.shape)
    print(cX) 
    print(cY)
    f = cX.size
    coordenadas = np.concatenate([cX.reshape(f,1),cY.reshape(f,1)],axis = 1)
    coordenadas = coordenadas.astype(int)

    fichero01 = camInicial
    raw_data = open(fichero01)
    caminoInicial = np.loadtxt(raw_data, delimiter=",").astype(int) 
    caminoInicial = caminoInicial - 1
    

    fichero02 = camFinal
    raw_data = open(fichero02)
    caminoFinal = np.loadtxt(raw_data, delimiter=",").astype(int) 
    caminoFinal = caminoFinal - 1
    plt.subplot(121)
    graficarCamino(coordenadas,caminoInicial,name)
    plt.subplot(122)
    graficarCamino(coordenadas,caminoFinal, name)
    #plt.savefig(os.path.join('static', name+".png"))
    plt.savefig("/home/luis/Documentos/Projects/Feria-AlgoritmoGenetico/public/static/"+name+".png")

    print("Longitud inicial : %s \n" % longitudes[0])
    print("Longitud final : %s \n" % longitudes[1])
    return longitudes[0], longitudes[1]


