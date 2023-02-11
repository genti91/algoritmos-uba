"""
Grafo no dirigido con peso, no autoreferenciable
"""
class Grafo:
    def __init__(self):
        self.vertices = {}
        self.cantidad = 0

    def agregar_vertice(self, v):
        if v in self.vertices:
            return False
        self.vertices[v] = {}
        self.cantidad += 1
        return True

    def eliminar_vertice(self, v):
        if self.vertices.pop(v, None) == None:
            return False
        self.cantidad -= 1
        return True

    def cantidad(self):
        return self.cantidad

    def agregar_arista(self, v, w, peso):
        if v not in self.vertices or w not in self.vertices or v == w:
            return False
        self.vertices[v][w] = peso
        self.vertices[w][v] = peso
        return True

    def eliminar_arista(self, v, w):
        if w not in self.vertices[v]:
            return False
        self.vertices[v].pop(w)
        self.vertices[w].pop(v)
        return True

    def lista_vertices(self):
        lista_vertices = []
        for v in self.vertices:
            lista_vertices.append(v)
        return lista_vertices

    def adyacentes(self, v):
        if v not in self.vertices:
            return None
        lista_adyacentes = []
        for w in self.vertices[v]:
            lista_adyacentes.append(w)
        return lista_adyacentes

    def conectados(self, v, w):
        if w in self.vertices[v]:
            return True
        return False

    def peso(self, v, w):
        if w not in self.vertices[v]:
            return False
        return self.vertices[v][w]

    def pertenece(self, v):
        if v not in self.vertices:
            return False
        return True
