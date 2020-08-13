import random

def iniciar_grafo(vertices):
    relaciones = {}
    cantidad = 0

    if not vertices: return relaciones, cantidad

    for e in vertices:
        relaciones[e] = {}
        cantidad += 1
    
    return relaciones, cantidad
        

class Grafo():
    def __init__(self, vertices=None):
        self.relaciones, self.cantidad = iniciar_grafo(vertices)

    def __str__(self):
        return str(self.relaciones)

    def __len__(self):
        return self.cantidad

    def __iter__(self):
        '''Devuelve un iterable de los vertices del grafo.
        '''
        return iter(self.relaciones)
        
    def adyacentes(self, v):
        return self.relaciones.get(v)

    def eliminar_vertice(self, elemento):
        return self.relaciones.pop(elemento)
        self.cantidad -= 1

    def agregar_vertice(self, elemento):
        self.relaciones[elemento] = {}
        self.cantidad += 1

    def obtener_vertice_aleatorio(self):
        vertices = list(self.relaciones.keys())
        return vertices[random.randrange(0, len(vertices))]

    def relacionar_dirigido(self, origen, destino, peso=1):
        self.relaciones[origen][destino] = peso
    
    def relacionar_no_dirigido(self, elemento1, elemento2, peso=1):
        self.relacionar_dirigido(elemento1, elemento2, peso)
        self.relacionar_dirigido(elemento2, elemento1, peso)

    def obtener_arista(self, v1, v2):
        peso = None
        ady_v1 = self.relaciones.get(v1)
        if ady_v1: peso = ady_v1.get(v2)
        
        if peso == None: return None, None
        return ((v1, v2), peso)

    def actualizar_arista(self, v1, v2, n_peso):
        (w1, w2), peso = self.obtener_arista(v1, v2)
        if peso == None: return False

        self.relaciones[v1][v2] = n_peso
        return True

    def pertenece(self, v):
        '''Recibe un vertice y devuelve true si este se encuentra en el
        grafo. De lo contrario, false.
        '''
        if not v: return False
        return self.relaciones.get(v) == v

    def obtener_vertices(self):
        return list(self.relaciones.keys())
