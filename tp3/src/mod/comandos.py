from lib.operaciones import dijkstra_sin_peso, imprimir_camino, cfc
from mod.mensajes import *

# Esta será una variable global donde se almacenaran las componentes fuertemente conexas
# del grafo que representa la red de internet en caso de que se ejecute el comando 
# conectividad, de lo contrario se mantendrá en None.

CFC = None

#########################################################################################
                                                                                        #
                                # FUNCIONES COMANDO                                     #
                                                                                        #
######################################################################################### 

def listar_comandos(funcionalidades):
    print()
    for comando in funcionalidades:
        if comando == "listar_comandos": continue
        print(comando)

def camino(grafo, origen, destino):
    padres, dist = dijkstra_sin_peso(grafo, origen, destino)
    if not padres and not dist: print(ERR_CAMINO)
    else: imprimir_camino(padres, destino)

def conectados(grafo, vertice):
    global CFC

    if not CFC: CFC = cfc(grafo)
    
    cant = 0
    componente = CFC.get(vertice)
    if not componente: print("No se encontro conectividad para esta página")
    else:
        # Hecho asi para contabilizar los articulos
        for pagina in componente:
            print(pagina)
            cant += 1    

        print(cant)

def ciclo():
    pass

def lectura():
    pass

def mas_importantes():
    pass
