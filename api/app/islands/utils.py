def generateTXT(body):
    f = open ('/home/luis/Documentos/Projects/Api-Parallel/parameters.txt','w')
    f.write(str(body['nCiudades'])+"\n")
    f.write(str(body['tanPoblacion'])+"\n")
    f.write(str(body['proCruce'])+"\n")
    f.write(str(body['proMutacion'])+"\n")
    f.write(str(body['nunMaxGen'])+"\n")
    f.close()
    f = open ('/home/luis/Documentos/Projects/Api-Parallel/coordenadasX-MPI.txt','w')
    f.write((str(body['x']).replace(" ","")).replace("[","").replace("]","").replace(",","\n")+"\n")
    f.close()
    f = open ('/home/luis/Documentos/Projects/Api-Parallel/coordenadasY-MPI.txt','w')
    f.write((str(body['y']).replace(" ","")).replace("[","").replace("]","").replace(",","\n")+"\n")
    f.close()
    