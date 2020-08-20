class _Nodo:
    def __init__(self, dato, prox):
        self.dato = dato
        self.prox = prox

class Cola:
    def __init__(self):
        self.frente = None
        self.ultimo = None

    def esta_vacia(self):
        return not self.ultimo

    def ver_frente(self):
        if self.esta_vacia(): raise IndexError("Cola vacia")
        
        return self.frente.dato

    def desencolar(self):
        if self.esta_vacia(): raise IndexError("Cola vacia")
        
        dato = self.frente.dato
        self.frente = self.frente.prox
        
        if not self.frente: self.ultimo = None
        
        return dato

    def encolar(self, x):
        nuevo = _Nodo(x, None)

        if self.esta_vacia(): self.frente = nuevo
        else: self.ultimo.prox = nuevo
        
        self.ultimo = nuevo