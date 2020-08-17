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

def imprimir_form_flecha(solucion):
    if solucion != []: print(solucion[0], end="")
    
    for i in range(1, len(solucion)):
        print(f" -> {solucion[i]}", end="")
    
    print()

def imprimir_form_coma(solucion):
    print(solucion[0], end="")
    for i in range(1, len(solucion)): print(f", {solucion[i]}", end="")
    print()

def imprimir_componente(componente):
    imprimir_form_coma(orden)

    print(len(componente))

def imprimir_lectura(orden, primera):
    solucion = list()

    while True:
        solucion.append(primera)
        if not orden[primera]: break
        primera = orden[primera]
    
    imprimir_form_coma(solucion)

def imprimir_ciclo(ciclo, destino):
    solucion = list()

    while True:
        solucion.insert(0, destino)
        
        if ciclo[destino] == solucion[len(solucion) - 1]:
            solucion.insert(0, ciclo[destino])
            break
        
        destino = ciclo[destino]

    imprimir_form_flecha(solucion)

def imprimir_camino(padres, destino, costo=None):
    solucion = list()
    costo_aux = 0

    while True:
        solucion.insert(0, destino)
        if not padres[destino]: break
        costo_aux += 1
        destino = padres[destino]
    
    imprimir_form_flecha(solucion)

    if not costo: print(f"Costo: {costo_aux}")
    else: print(f"Costo: {costo}")

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
    orden, primera = orden_valido(grafo, paginas)
    
    if not orden: print(ERR_LECTURA)
    imprimir_lectura(orden, primera)

def diametro(grafo):
    diametro, destino, camino = obtener_diametro(grafo)
    imprimir_camino(camino, destino, diametro)

def todos_en_rango(grafo, pagina, n):
    cantidad = rango_n(grafo, pagina, n)
    print(cantidad)

def navegacion(grafo, origen):
    camino = camino_dfs(grafo, origen)
    imprimir_form_flecha(camino)

def coeficiente_de_clustering(grafo, pagina, cant_params):
    if cant_params == 0:
        promedio = obtener_promedio_clustering(grafo)
        print(promedio)
    else:
        coef = obtener_coef_clustering(grafo, pagina[0])
        print(coef)

def clear():
    os.system("clear")

def mas_importantes():
    pass
