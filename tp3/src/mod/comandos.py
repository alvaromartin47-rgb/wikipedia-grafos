import os
from lib.operaciones import camino_minimo_bfs, cfc, backtracking, rango_n, camino_dfs, obtener_promedio_clustering, obtener_coef_clustering, orden_valido
from mod.mensajes import *

# Esta será una variable global donde se almacenaran las componentes fuertemente conexas
# del grafo que representa la red de internet en caso de que se ejecute el comando 
# conectividad, de lo contrario se mantendrá en None.

CFC = None

#########################################################################################
                                                                                        #
                                # FUNCIONES IMPRIMIR                                    #
                                                                                        #
######################################################################################### 

def imprimir_orden(orden):
    print(orden[0], end="")
    for i in range(1, len(orden)): print(f", {orden[i]}", end="")
    print()

def imprimir_componente(componente):
    print(componente[0], end="")
    for i in range(1, len(componente)): print(f", {componente[i]}", end="")
    print()

    print(len(componente))

def imprimir_ciclo(padres, destino):
    solucion = list()

    while True:
        solucion.insert(0, destino)
        
        if padres[destino] == solucion[len(solucion) - 1]:
            solucion.insert(0, padres[destino])
            break
        
        destino = padres[destino]

    if solucion != []: print(solucion[0], end="")
    for i in range(1, len(solucion)):
        print(f" -> {solucion[i]}", end="")
    print()

def imprimir_camino(padres, destino):
    solucion = list()
    costo = 0

    while True:
        solucion.insert(0, destino)
        if not padres[destino]: break
        costo += 1
        destino = padres[destino]
    
    if solucion != []: print(solucion[0], end="")
    
    for i in range(1, len(solucion)):
        print(f" -> {solucion[i]}", end="")
    print()

    print(f"Costo: {costo}")

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
    padres, distancia = camino_minimo_bfs(grafo, origen, destino)
    if not padres and not dist: print(ERR_CAMINO)
    else: imprimir_camino(padres, destino)

def conectados(grafo, vertice):
    global CFC
    if not CFC: CFC = cfc(grafo)

    componente = CFC.get(vertice)
    
    if not componente:
        print(ERR_CONEC)
        return

    imprimir_componente(componente)

def ciclo(grafo, origen, n):
    if not grafo.pertenecen(origen): print(ERR_CICLO)
    
    hay_ciclo, ciclo = backtracking(grafo, origen, int(n))
    
    if hay_ciclo: imprimir_ciclo(ciclo, origen)
    else: print(ERR_CICLO)
    
def lectura(grafo, paginas):
    orden = orden_valido(grafo, paginas)
    print(orden)
    # if not orden: print(ERR_LECTURA)
    # imprimir_orden(orden)

def mas_importantes():
    pass

def diametro(grafo):
    diametro, destino, camino = obtener_diametro(grafo)
    imprimir_camino(camino, destino)
    print(f"Costo: {diametro}")

def todos_en_rango(grafo, pagina, n):
    cant = rango_n(grafo, pagina, n)
    print(cant)

def navegacion(grafo, origen):
    camino = camino_dfs(grafo, origen)
    
    if camino != []: print(camino[0], end="")

    for i in range(1, len(camino)):
        print(f" -> {camino[i]} ", end="")
    print()

def coeficiente_de_clustering(grafo, pagina, cant_params):
    if cant_params == 0:
        promedio = obtener_promedio_clustering(grafo)
        print(promedio)
    else:
        coef = obtener_coef_clustering(grafo, pagina[0])
        print(coef)

def clear():
    os.system("clear")
