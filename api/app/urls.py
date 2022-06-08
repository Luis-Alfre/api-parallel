from api.app.islands.views import PetriNets
#importar todas las rutas aqui

def initialize_routes(api):
    api.add_resource(PetriNets, '/petri')