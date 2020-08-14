import sys
import math
from lib.grafo.grafo import Grafo
from collections import defaultdict
from lib.cola.cola import Cola
from lib.pila.pila import Pila

SEPARADOR = f"{chr(9)}"
sys.setrecursionlimit(50000)

#########################################################################################
                                                                                        #
                        # LIBRERIA DE OPERACIONES SOBRE GRAFOS                          #
                                                                                        #
######################################################################################### 

def obtener_comandos(ruta_archivo):
    '''Lee el contenido de _ruta_archivo_ en formato .csv
    y devuelve un diccionario con el contenido de las primeras
    dos columnas del archivo. Como clave tendrá la primer columna de
    cada linea y como valor la segunda.
    Pre: el archivo tiene extensión .csv, existe y tiene al menos dos
    columnas.
    '''
    funcionalidades = dict()
    
    with open(ruta_archivo) as archivo:
        for linea in archivo:
            linea = linea.rstrip("\n").split(",")
            funcionalidades[linea[0]] = linea[1]
    
    return funcionalidades


def cargar_contenido(ruta_archivo):
    '''Lee el contenido de _ruta_archivo_ y devuelve un grafo
    dirigido donde cada vertice es una página y cada arista es
    el enlace de una pagina a otra.
    '''
    red_internet = Grafo()
    with open(ruta_archivo) as archivo:
        for linea in archivo:
            linea = linea.rstrip("\n").split(SEPARADOR)
            
            pagina = linea[0]

            if not red_internet.pertenece(pagina): red_internet.agregar_vertice(pagina)
            
            for i in range(1, len(linea)):
                if not red_internet.pertenece(linea[i]): red_internet.agregar_vertice(linea[i])
                red_internet.relacionar_dirigido(pagina, linea[i])
    
    return red_internet


def imprimir_camino(padres, destino):
    solucion = list()

    while True:
        solucion.insert(0, destino)
        if not padres[destino]: break
        destino = padres[destino]

    if solucion != []: print(solucion[0], end="")
    
    for i in range(1, len(solucion)):
        print(f" -> {solucion[i]}", end="")
    
    print()


def camino_minimo_bfs(grafo, origen, destino):
    padres = dict()
    cola = Cola()
    dist = defaultdict(lambda : math.inf)

    padres[origen] = None
    dist[origen] = 0
    cola.encolar((origen, dist[origen]))

    while not cola.esta_vacia():
        v, d = cola.desencolar()
        if v == destino: return padres, dist

        for w in grafo.adyacentes(v):
            (v, w), peso = grafo.obtener_arista(v, w)

            if dist[v] + peso < dist[w]:
                dist[w] = dist[v] + peso
                padres[w] = v
                cola.encolar((w, dist[w]))
    
    return None, None


def agregar_componente(componentes, v, pila):
    componente = list()

    while not pila.esta_vacia():
        vertice = pila.desapilar()
        componente.append(vertice)
        if vertice == v: break

    for vertice in componente:
        componentes[vertice] = componente


def _cfc(grafo, v, visitados, mb, orden, pila, componentes):
    visitados.add(v)
    pila.apilar(v)
    mb[v] = orden[v]
    
    for w in grafo.adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            _cfc(grafo, w, visitados, mb, orden, pila, componentes)
        
        if pila.pertenece(w): mb[v] = min(mb[v], mb[w])

    if mb[v] == orden[v] and not pila.esta_vacia(): agregar_componente(componentes, v, pila)

def cfc(grafo):
    visitados = set()
    mb = {}
    orden = {}
    pila = Pila()
    componentes = {}

    for v in grafo:
        if v not in visitados:
            orden[v] = 0
            _cfc(grafo, v, visitados, mb, orden, pila, componentes)

    return componentes


def calcular_grados_entrada(grafo):
    grados_entrada = dict()
    for v in grafo: grados_entrada[v] = 0
    
    for v in grafo:
        for w in grafo.adyacentes(v):
            grados_entrada[w] += 1
    
    return grados_entrada

def orden_topologico_bfs(grafo):
    c = Cola()
    orden_topologico = list()

    grados_entrada = calcular_grados_entrada(grafo)

    for v in grafo:
        if grados_entrada[v] == 0: c.encolar(v)

    while not c.esta_vacia():
        v = c.desencolar()
        orden_topologico.append(v)
        
        for w in grafo.adyacentes(v):
            grados_entrada[w] -= 1
            
            if grados_entrada[w] == 0: c.encolar(w)
    
    return orden_topologico