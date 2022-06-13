#model
# flask
from flask import Response, request
from flask_restful import Resource

import shared.importGenetic as importGenetic 
from api.app.islands.graficarTSP import graph

class PetriNets(Resource):
    
    def post(self):
        body = request.get_json()
        importGenetic.suma_enteros(body['nCiudades'],
                             body['tanPoblacion'],
                             body['proCruce'],
                             body['proMutacion'],
                             body['nunMaxGen'],
                             body['x'],
                             body['y'])
        
        graph('../Api-Parallel/coordenadasX.txt',
              '../Api-Parallel/coordenadasY.txt',
              '../Api-Parallel/longitudesIsla1.txt',
              '../Api-Parallel/caminoInicialIsla1.txt',
              '../Api-Parallel/caminoFinalIsla1.txt',
              'Isla 1')
        graph('../Api-Parallel/coordenadasX.txt',
              '../Api-Parallel/coordenadasY.txt',
              '../Api-Parallel/longitudesIsla2.txt',
              '../Api-Parallel/caminoInicialIsla2.txt',
              '../Api-Parallel/caminoFinalIsla2.txt',
              'Isla 2')
        graph('../Api-Parallel/coordenadasX.txt',
              '../Api-Parallel/coordenadasY.txt',
              '../Api-Parallel/longitudesIsla3.txt',
              '../Api-Parallel/caminoInicialIsla3.txt',
              '../Api-Parallel/caminoFinalIsla3.txt',
              'Isla 3')
        graph('../Api-Parallel/coordenadasX.txt',
              '../Api-Parallel/coordenadasY.txt',
              '../Api-Parallel/longitudesIsla4.txt',
              '../Api-Parallel/caminoInicialIsla4.txt',
              '../Api-Parallel/caminoFinalIsla4.txt',
              'Isla 4')
        
        return {"Isla 1":"/Projects/Api-Parallel/Isla 1.png",
                "Isla 2":"/Projects/Api-Parallel/Isla 2.png",
                "Isla 3":"/Projects/Api-Parallel/Isla 3.png",
                "Isla 4":"/Projects/Api-Parallel/Isla 4.png"
                }, 200


