# model
# flask
import subprocess
from flask import request
from flask_restful import Resource
from api.app.code.geneticMPI.run import runGenetic
import shared.importGenetic as importGenetic
from api.app.islands.graficarTSP import graph
from api.app.islands.utils import generateTXT

class PetriNets(Resource):
    def post(self):
      body = request.get_json()
      print(body)
      if int(body["library"]) == 1:
            importGenetic.genetic(
                int(body["nCiudades"]),
                int(body["tanPoblacion"]),
                float(body["proCruce"]),
                float(body["proMutacion"]),
                int(body["nunMaxGen"]),
                body["x"],
                body["y"],
            )

            inicial1, final1 = graph(
                "../Api-Parallel/coordenadasX.txt",
                "../Api-Parallel/coordenadasY.txt",
                "../Api-Parallel/longitudesIsla1.txt",
                "../Api-Parallel/caminoInicialIsla1.txt",
                "../Api-Parallel/caminoFinalIsla1.txt",
                "Isla1",
            )
            inicial2, final2 = graph(
                "../Api-Parallel/coordenadasX.txt",
                "../Api-Parallel/coordenadasY.txt",
                "../Api-Parallel/longitudesIsla2.txt",
                "../Api-Parallel/caminoInicialIsla2.txt",
                "../Api-Parallel/caminoFinalIsla2.txt",
                "Isla2",
            )
            inicial3, final3 = graph(
                "../Api-Parallel/coordenadasX.txt",
                "../Api-Parallel/coordenadasY.txt",
                "../Api-Parallel/longitudesIsla3.txt",
                "../Api-Parallel/caminoInicialIsla3.txt",
                "../Api-Parallel/caminoFinalIsla3.txt",
                "Isla3",
            )
            inicial4, final4 = graph(
                "../Api-Parallel/coordenadasX.txt",
                "../Api-Parallel/coordenadasY.txt",
                "../Api-Parallel/longitudesIsla4.txt",
                "../Api-Parallel/caminoInicialIsla4.txt",
                "../Api-Parallel/caminoFinalIsla4.txt",
                "Isla4",
            )

            return {
                "Isla1": {
                    "ruta": "/static/Isla1.png",
                    "inicial": inicial1,
                    "final": final1,
                },
                "Isla2": {
                    "ruta": "/static/Isla2.png",
                    "inicial": inicial2,
                    "final": final2,
                },
                "Isla3": {
                    "ruta": "/static/Isla3.png",
                    "inicial": inicial3,
                    "final": final3,
                },
                "Isla4": {
                    "ruta": "/static/Isla4.png",
                    "inicial": inicial4,
                    "final": final4,
                },
            }, 200
      else:
            generateTXT(body)
            runGenetic()
            
            inicial, final = graph(
                "../Api-Parallel/coordenadasX-incial.txt",
                "../Api-Parallel/coordenadasY-incial.txt",
                "../Api-Parallel/longitudesIsla-MPI.txt",
                "../Api-Parallel/caminoInicial-MPI.txt",
                "../Api-Parallel/caminoFinal-MPI.txt",
                "Isla1-MPI",
            )
            
            return {
                    "ruta": "/static/Isla1-MPI.png",
                    "inicial": inicial,
                    "final": final,
                },200
            




