import time
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
            
            if len(linea[1]) == 3 and linea[1][1] == ":":
                if linea[1][0].isdigit() and linea[1][2].isdigit():
                    funcionalidades[linea[0]] = (int(linea[1][0]), int(linea[1][2]) + 1)
            
            elif linea[1].isdigit():
                funcionalidades[linea[0]] = (int(linea[1][0]), int(linea[1][0]) + 1)
            
            else: funcionalidades[linea[0]] = linea[1]
    
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
            
            if linea[0] == "": continue
            pagina = linea[0]

            if not red_internet.pertenecen(pagina): red_internet.agregar_vertice(pagina)
            
            for i in range(1, len(linea)):
                if not red_internet.pertenecen(linea[i]): red_internet.agregar_vertice(linea[i])
                red_internet.agregar_arista_dirigido(pagina, linea[i])

    return red_internet



def camino_minimo_bfs(grafo, origen, destino=None):
    padres = dict()
    cola = Cola()
    dist = defaultdict(lambda : math.inf)

    padres[origen] = None
    dist[origen] = 0
    cola.encolar((origen, dist[origen]))

    while not cola.esta_vacia():
        v, d = cola.desencolar()
        if v == destino: return padres, dist

        for w in grafo.obtener_adyacentes(v):
            (v, w), peso = grafo.obtener_arista(v, w)

            if dist[v] + peso < dist[w]:
                dist[w] = dist[v] + peso
                padres[w] = v
                cola.encolar((w, dist[w]))
    
    if not destino: return padres, dist
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
    
    for w in grafo.obtener_adyacentes(v):
        if w not in visitados:
            orden[w] = orden[v] + 1
            _cfc(grafo, w, visitados, mb, orden, pila, componentes)
        
        if pila.pertenece(w): mb[v] = min(mb[v], mb[w])

    if mb[v] == orden[v] and not pila.esta_vacia(): agregar_componente(componentes, v, pila)

def cfc(grafo):
    visitados = set()
    mb = dict()
    orden = dict()
    pila = Pila()
    componentes = dict()

    for v in grafo:
        if v not in visitados:
            orden[v] = 0
            _cfc(grafo, v, visitados, mb, orden, pila, componentes)

    return componentes


def buscar_orden(grafo, p, paginas, n, padres, visitados):
    # time.sleep(1)
    print(f"Laburando con {p}")
    print(f"n es {n} y la cantidad de paginas es {len(paginas)}")
    if n == len(paginas): 
        print("Hay coincidencia! devuelvo True")
        return True
    
    for pagina in paginas:
        print(f"Laburando con la siguiente pagina, {pagina}")
        if not grafo.estan_conectados(p, pagina) or pagina in visitados: 
            print(f"Continuo con la siguiente pagina")    
            continue

        visitados.add(pagina)
        padres[pagina] = p
        print(f"{pagina} se encuentra en los adyacentes de {p} y aun no esta visitado, llamo recursivamente")
        
        hay_solucion =  buscar_orden(grafo, pagina, paginas, n + 1, padres, visitados)
        print(f"Volvi en recursividad, saco de visitados a {pagina}. N es {n}")
        visitados.remove(pagina)
        
        if hay_solucion: return True
        print(f"No hubo coincidencias, continuo con la siguiente pagina")
    
    return False

def orden_valido(grafo, paginas):
    padres = dict()
    visitados = set()

    for pagina in paginas:
        padres[pagina] = None
        visitados.add(pagina)
        hay_orden = buscar_orden(grafo, pagina, paginas, 1, padres, visitados)
        if hay_orden: return padres
    
    return None

def _backtracking(grafo, origen, n, v_act, n_act, padres, visitados):
    if n_act == n and n_act > 0: return v_act == origen
    if v_act == origen and n_act > 0: return False
    
    for w in grafo.obtener_adyacentes(v_act):
        if (w in visitados and w != origen) or w == padres[v_act]: continue
    
        padres[w] = v_act
        visitados.add(w)

        hay_solucion = _backtracking(grafo, origen, n, w, n_act + 1, padres, visitados)
        
        ultimo = visitados.remove(w)

        if hay_solucion: return True

    return False

def backtracking(grafo, origen, n):
    visitados = set()
    padres = dict()
    n_act = 0
    
    padres[origen] = None
    visitados.add(origen)

    return _backtracking(grafo, origen, n, origen, n_act, padres, visitados), padres

def obtener_diametro(grafo):
    """Devuelve el diametro del grafo, el vértice que se encuentra a esa distancia y un dic para poder reconstruir el camino. Recibe como parametro el grafo.
    Pre: el grafo fue creado.
    """
    max_dist_min = 0
    destino = None
    camino = None

    for v in grafo:
        padres, distancia = camino_minimo_bfs(grafo, v)
        for w in distancia:
            if distancia[w] > max_dist_min:
                max_dist_min = distancia[w]
                destino = w
                camino = padres

    return max_dist_min, destino, camino

def rango_n(grafo, vertice, n):
    """Devuelve la cantidad de vértices que se encuentra a distancia n de un vértice. Recibe como parámetro el grafo,
    un vértice y un número n.
    Pre: el grafo fue creado y el vértice se encuentra en él.
    """
    padres, distancia = camino_minimo_bfs(grafo, vertice)
    contador = 0

    for w in distancia:
        if distancia[w] == n:
            contador += 1

    return contador

def _camino_dfs(grafo, v, contador, camino):
    if contador >= 20: return
    camino.append(v)
    adyacentes = grafo.obtener_adyacentes(v)
    if len(adyacentes) == 0: return
    _camino_dfs(grafo, adyacentes[0], contador + 1, camino)

def camino_dfs(grafo, origen):
    """Devuelve un camino desde un vértice hasta otro, sólo accediendo a la primer arista de los vértices. El camino termina
    cuando el último vértice no tiene adyacentes o cuando se recorriendo 20 vértices. Recibe como parámetro el grafo y un origen.
    Pre: el grafo fue creado y el origen pertenece a él.
    """
    contador = 0
    camino = []
    _camino_dfs(grafo, origen, contador, camino)
    return camino

def obtener_promedio_clustering(grafo):
    """Devuelve el promedio de los coeficientes Clustering de todos los vértices del grafo.
    Pre: el grafo fue creado.
    """
    sumatoria = 0
    n = len(grafo)

    for v in grafo:
        sumatoria += obtener_coef_clustering(grafo, v)

    return round(sumatoria / n, 3)

def obtener_coef_clustering(grafo, vertice):
    """Devuelve el coeficiente Clustering de un vértice. Recibe como parámetros el grafo y el vértice en cuestión.
    Pre: el grafo fue creado y el vértice pertenece a él.
    """
    cant = 0
    adyacentes = grafo.obtener_adyacentes(vertice)
    grado_salida = len(adyacentes)
    
    if grado_salida < 2: return 0
    
    for v in adyacentes:
        for w in adyacentes:
            if v == w: continue
            if w in grafo.obtener_adyacentes(v): cant += 1
    
    denominador = grado_salida * (grado_salida - 1)
    coef = cant / denominador
    return round(coef, 3)
