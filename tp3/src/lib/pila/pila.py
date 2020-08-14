class Pila:
	'''
	Representa una pila con los metodos apilar, desapilar, verificar si está vacia.
	'''
	def __init__(self):
		'''
		Crea una pila vacia.
		'''
		self.items = []
		self.items_2 = set()

	def __str__(self):
		return str(self.items)

	def esta_vacia(self):
		'''
		Devuelve True si la lista esta vacia, de lo contrario False.
		'''
		return len(self.items) == 0

	def apilar(self, x):
		'''
		Apila un elemento 'x'.
		'''
		self.items.append(x)
		self.items_2.add(x)

	def desapilar(self):
		'''
		Devuelve el elemento tope y lo elimina de la pila.
		Si la pila está vacia, levanta una excepción.
		'''
		if self.esta_vacia():
			raise IndexError('La pila está vacía.')

		borrado = self.items.pop()
		self.items_2.remove(borrado)
		return borrado

	def pertenece(self, elemento):
		return elemento in self.items_2

	def ver_tope(self):
		'''
		Devuelve el ultimo elemento que se agrego a la pila.
		'''
		if len(self.items) == 0:
			return None
		return self.items[len(self.items)-1]
		