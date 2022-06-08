#model
# flask
from flask import Response, request
from flask_wtf import FlaskForm
from flask_restful import Resource
import json
import ctypes

import example 

class PetriNets(Resource):
    
    def post(self):
        body = request.get_json()
        example.suma_enteros(body['nCiudades'],
                             body['tanPoblacion'],
                             body['proCruce'],
                             body['proMutacion'],
                             body['nunMaxGen'])
        return {"New State":"si"}, 200


