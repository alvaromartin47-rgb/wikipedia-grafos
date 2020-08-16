import os
from lib.operaciones import camino_minimo_bfs, imprimir_camino, cfc, backtracking, imprimir_ciclo
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
    padres, dist = camino_minimo_bfs(grafo, origen, destino)
    if not padres and not dist: print(ERR_CAMINO)
    else: imprimir_camino(padres, destino)

def conectados(grafo, vertice):
    global CFC
    if not CFC: CFC = cfc(grafo)

    componente = CFC.get(vertice)
    
    if not componente:
        print(ERR_CONEC)
        return

    print(componente[0], end="")
    for i in range(1, len(componente)): print(f", {componente[i]}", end="")
    print()

    print(len(componente))

def ciclo(grafo, origen, n):
    if not grafo.pertenecen(origen): print(ERR_CICLO)
    
    hay_ciclo, ciclo = backtracking(grafo, origen, int(n))
    
    if hay_ciclo: imprimir_ciclo(ciclo, origen)
    
    else: print(ERR_CICLO)
    
def lectura():
    pass

def mas_importantes():
    pass

def clear():
    os.system("clear")