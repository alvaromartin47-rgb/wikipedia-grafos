class Pila:
	'''
	Representa una pila con los metodos apilar, desapilar, verificar si está vacia.
	'''
	def __init__(self):
		'''
		Crea una pila vacia.
		'''
		self.items = []
		self.aux = set()
		self.cantidad = 0

	def __str__(self):
		return str(self.items)

	def __len__(self):
		return self.cantidad

	def __contains__(self, v):
		return v in self.aux

	def esta_vacia(self):
		'''
		Devuelve True si la pila esta vacia, de lo contrario False.
		'''
		return self.cantidad == 0

	def apilar(self, x):
		'''
		Apila un elemento 'x'.
		'''
		self.items.append(x)
		self.aux.add(x)
		self.cantidad += 1

	def desapilar(self):
		'''
		Devuelve el elemento tope y lo elimina de la pila.
		Si la pila está vacia, levanta una excepción.
		'''
		if self.esta_vacia(): raise IndexError('La pila está vacía.')

		borrado = self.items.pop()
		self.aux.remove(borrado)
		self.cantidad -= 1
		
		return borrado

	def ver_tope(self):
		'''
		Devuelve el ultimo elemento que se agrego a la pila.
		'''
		if self.esta_vacia(): return None
		return self.items[-1]
		