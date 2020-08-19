import os
from decimal import Decimal
from mod.pantalla import *
from lib.operaciones import camino_minimo_bfs, cfc, obtener_ciclo_n, rango_n, camino_dfs, obtener_diametro, obtener_promedio_clustering, obtener_coef_clustering, label_propagation, page_rank
from mod.mensajes import *

# Esta será una variable global donde se almacenaran las componentes fuertemente conexas
# del grafo que representa la red de internet en caso de que se ejecute el comando 
# conectividad, de lo contrario se mantendrá en None.

CFC = None

# En esta variable global se almacenará el ranking de paginas mas importantes de la red si
# el comando mas_importantes es llamado, de lo contrario se mantendrá en None.

RANK = None

#########################################################################################
                                                                                        #
                                # FUNCIONES COMANDO                                     #
                                                                                        #
######################################################################################### 

def listar_comandos(funcionalidades):
    """Recibe una lista de comandos validos para el programa e
    los imprime por consola.
    """
    print()
    for comando in funcionalidades:
        if comando == "listar_operaciones": continue
        print(comando)

def camino(red_internet, origen, destino):
    """Recibe un grafo en forma de red de internet y obtiene la minima cantidad
    de paginas que hay para llegar desde _origen_ a _destino_. Imprime los resultados
    por consola.
    Pre: la red fue creada.
    """
    padres, distancia = camino_minimo_bfs(red_internet, origen, destino)
    
    if not padres and not distancia: print(ERR_CAMINO)
    else: imprimir_camino(padres, destino)

def mas_importantes(red_internet, n):
    """Recibe un grafo en forma de red de internet y un valor _n_, obtiene un ranking basado
    en las paginas mas importantes de toda la red utilizando el algoritmo PageRank e imprime por
    consola las _n_ paginas mas importantes.
    Pre: el grafo fue creado.
    """
    global RANK
    if not RANK: RANK = page_rank(red_internet, 15)

    imprimir_form_coma(RANK, n)

def conectados(red_internet, origen):
    """Recibe un grafo en forma de red de internet, calcula la cantidad de paginas
    conectadas entre sí de tal forma en la que cada pagina puede llegar hasta _origen_ y las
    muestra por consola. En caso de no haber paginas que cumplan esto, imprime un mensaje por
    consola indicándolo.
    Pre: el grafo fue creado.
    """
    global CFC
    if not CFC: CFC = cfc(red_internet)

    componente = CFC.get(origen)
    
    if not componente:
        print(ERR_CONEC)
        return

    imprimir_componente(componente, len(componente))
    print(len(componente))

def ciclo(red_internet, origen, n):
    """Recibe un grafo en forma de red de internet y un largo _n_.
    Busca un ciclo de largo _n_ dentro de la red partiendo desde _origen_, si no
    existe solución muestra un mensaje por consola, si existe, muestra la solucion.
    Pre: el grafo fue creado.
    """
    if not red_internet.pertenecen(origen): print(ERR_CICLO)
    
    hay_ciclo, ciclo = obtener_ciclo_n(red_internet, origen, int(n))
    
    if hay_ciclo: imprimir_ciclo(ciclo, origen)
    else: print(ERR_CICLO)

def diametro(red_internet):
    """Recibe un grafo en forma de red de internet, calcula el camino mas largo de
    toda la red y lo muestra por consola.
    Pre: el grafo fue creado.
    """
    diametro, destino, camino = obtener_diametro(red_internet)
    
    imprimir_camino(camino, destino, diametro)

def todos_en_rango(red_internet, origen, n):
    """Recibe un grafo en forma de red de internet un origen y un entero n, muestra la cantidad
    paginas que se encuentren exactamente a _n_ links de _origen_.
    Pre: el grafo fue creado y la pagina origen pertenece a la red.
    """
    cantidad = rango_n(red_internet, origen, n)
    
    print(cantidad)

def navegacion(red_internet, origen):
    """Recibe un grafo en forma de red de internet y un origen, muestra por consola el camino
    realizado a través de la red utilizando solamente el primer link de cada pagina
    comenzando desde _origen_ hasta que alguna de las paginas no contenga links o bien,
    el largo del recorrido sea 20.
    Pre: el grafo fue creado y _origen_ pertenece a la red.
    """
    camino = camino_dfs(red_internet, origen)
    
    imprimir_form_flecha(camino)

def coeficiente_de_clustering(red_internet, origen, cant_params):
    """Recibe un grafo en forma de red de internet, un origen y una cantidad de parametros entre
    0 y 1. Si la cantidad de parametros no tiene elementos calcula el coeficiente de clustering a toda
    la red (promedio) y lo muestra por consola. Si la cantidad de parametros es 1 solo se lo calculará
    a _origen_.
    Pre: el grafo fue creado y _origen_ pertenece a la red.
    """
    if cant_params == 0:
        res = obtener_promedio_clustering(red_internet)
    else:
        res = obtener_coef_clustering(red_internet, origen[0])
    
    print("{:.3f}".format(res))
    
def obtener_comunidad(grafo, pagina):
    """Recibe un grafo en forma de red de internet y una página web. Devuelve todas las páginas
    que pertenecen a la misma comunidad que la página pasada por parámetro.
    Pre: el grafo fue creado y _pagina_ pertenece a la red.
    """
    comunidades = label_propagation(grafo)
    comunidad_pagina = comunidades[pagina]
    comunidad = []

    for c in comunidades:
        if comunidades[c] == comunidad_pagina: comunidad.append(c)
    
    imprimir_form_coma(comunidad, len(comunidad))

def clear():
    """Limpia la consola"""
    
    os.system("clear")
