from mod.mensajes import MENSAJE_INDEX_ERROR

class Vertice:
    """Representación de la estructura de datos vértice."""
    
    def __init__(self, clave):
        """Constructor del Vertice. Recibe por parametro el nombre del vértice."""
        
        self.clave = clave
        self.vecinos = {}
    
    def __str__(self):
        """Método para representar el Vértice al imprimir."""
        
        return f"{self.clave} -> {self.vecinos}"

    def obtener_clave(self):
        """Devuelve la clave del vértice."""
        
        return self.clave


    def agregar_adyacente(self, vertice, peso):
        """Une el vértice con otro pasado por parametro.
        Post: se unieron dos vértices.
        """
        self.vecinos[vertice] = peso

        return True

    def eliminar_adyacente(self, vertice):
        """Elimina la conexión entre dos vértices. 
        Pre: los vértices fueron creados.
        Post: se eliminó la conexión entre dos vértices.
        """
        self.vecinos.pop(vertice)

    def es_adyacente(self, vertice):
        """Devuelve True _vertice_ se encuentra en los vecinos, de lo
        contrario False.
        """
        return vertice in self.vecinos

    def adyacentes(self):
        """Devuelve una lista de adyacentes de un Vertice."""
        
        return list(self.vecinos.keys())


class Grafo:
    """Representación del TDA Grafo."""
    
    def __init__(self, es_dirigido):
        """Constructor de un grafo vacío. Recibe un boleano indicado si es dirigido
        o no."""

        self.es_dirigido = es_dirigido
        self.vertices = {}
        self.cantidad = 0

    def __iter__(self):
        """Método de iteración del grafo."""
        
        return iter(self.vertices)
    
    def __len__(self):
        """Devuelve la cantidad de elementos."""
        
        return self.cantidad
    
    def __str__(self):
        """Método para representar el Grafo al imprimir."""
        
        res = ""
        for v in self.vertices:
            res += f"{v} -> {self.vertices[v].adyacentes()}\n"
        return res

    def obtener_vertices(self):
        """Devuelve una lista con todas los vertices del grafo."""
        
        return list(self.vertices.keys())
    
    def pertenecen(self, *args):
        """Recibe una cantidad n de argumentos y devuelve True si todos pertenen al grafo,
        de lo contrario False.
        """
        for vertice in args:
            if not vertice in self.vertices: return False
        
        return True

    def agregar_vertice(self, clave_vertice):
        """Agrega un vértice al grafo. Devuelve True si se pudo agregar el vértice y en el caso
        de que exista un vértice con el mismo nombre, no se agrega y devuelve False.
        Post: se agregó un vértice al grafo.
        """
        
        if self.pertenecen(clave_vertice): return False
        
        vertice = Vertice(clave_vertice)
        self.vertices[clave_vertice] = vertice
        self.cantidad += 1
        
        return True
        
    def agregar_arista_dirigido(self, vertice_A, vertice_B, peso = 1):
        """Agrega una arista entre vertice_A y vertice_B (vertice_A -> vertice_B). 
        Recibe por parametro los vértices a unir. Devuelve True si se logró agregar la arista
        o False si los vertices no pertenecen al grafo. Si la arista ya se encontraba la actualiza.
        En caso de que se quiere agregar peso a la arista, se le pasa como tercer parámetro el peso.
        Post: se agregó una arista al grafo.
        """
        if not self.pertenecen(vertice_A, vertice_B): return False

        if not self.es_dirigido:
            vertice2 = self.vertices[vertice_B]
            vertice2.agregar_adyacente(vertice_A, peso)

        vertice1 = self.vertices[vertice_A]
        vertice1.agregar_adyacente(vertice_B, peso)
        return True

    def estan_conectados(self, vertice_A, vertice_B):
        """Devuelve un booleano dependiendo si el vértice A está conectado con el vértice B. (A -> B)."""
        
        if not self.pertenecen(vertice_A, vertice_B): raise IndexError(MENSAJE_INDEX_ERROR)
        return self.vertices[vertice_A].es_adyacente(vertice_B)
    
    def eliminar_vertice(self, clave_vertice):
        """Elimina un vértice del grafo. Devuelve un diccionario de las relaciones que tenia ese vertice (todos
        sus adyacentes). Si el vértice no se encuentra en el grafo se lanza una excepción.
        Post: se eliminó el vertice del grafo.
        """
        if not self.pertenecen(clave_vertice): raise IndexError(MENSAJE_INDEX_ERROR)

        vertice = self.vertices.pop(clave_vertice)
        
        self.cantidad -= 1
        return clave_vertice

    def eliminar_arista(self, vertice_A, vertice_B):
        """Elimina la arista que une el vértice A del vértice B (A -> B). Devuelve True si se logró
        eliminarla y, en caso contrario, False.
        Post: se eliminó la arista que une A con B.
        """
        if not self.pertenecen(vertice_A, vertice_B): raise IndexError(MENSAJE_INDEX_ERROR)
        
        ok = True

        if not self.es_dirigido: ok = self.vertices[vertice_B].es_adyacente(vertice_A)
        ok = self.vertices[vertice_A].es_adyacente(vertice_B)

        if not ok: return False
        
        if not self.es_dirigido: self.vertices[vertice_B].eliminar_adyacente(vertice_A)
        self.vertices[vertice_A].eliminar_adyacente(vertice_B)
        
        return True
    
    def obtener_adyacentes(self, vertice):
        """Devuelve todos los adyacentes de un vértice que se encuentra en el grafo."""
        
        if not self.pertenecen(vertice): raise IndexError(MENSAJE_INDEX_ERROR)
        
        return self.vertices[vertice].adyacentes()

    def obtener_arista(self, v, w):
        """Devuelve la arista entre _v_ y _w_ en forma de tupla ((v, w), peso). En caso de
        que los vertices no existan lanza excepcion, y, si los vertices existen pero no estan
        conectados devuelve una tupla con None.
        """
        if not self.pertenecen(v, w): raise IndexError(MENSAJE_INDEX_ERROR)
        
        if not self.estan_conectados(v, w): return None, None
        return (v, w), aristas[v][w]
