class Cola:
	'''
	Representa a una cola, con operaciones de encolar y desencolar. El primero en ser encolado es también el primero
	en ser desencolado.
	'''
	def __init__(self):
		'''
		Crea una cola vacía.
		'''
		self.items = []
	
	def encolar(self, x):
		'''
		Encola el elemento x.
		'''
		self.items.append(x)

	def desencolar(self):
		'''
		Elimina el primer elemento de la cola y devuelve su valor. Si la cola está vacía, levanta ValueError.
		'''
		if self.esta_vacia():
			raise ValueError("La cola está vacía")
		return self.items.pop(0)

	def esta_vacia(self):
		'''
		Devuelve True si la cola esta vacía, False si no.
		'''
		return len(self.items) == 0

	def ver_frente(self):
		'''
		Devuelve el elemento que esta al frente en la cola.
		El ultimo de la lista. Si esta vacia devuelve None.
		'''
		if len(self.items) == 0:
			return None

		return self.items[0]