from collections import deque
import sys
from random import randint

# Busca el camino mas corto desde origen (cancion) a destino.
# Imprime por salida estandar las canciones del camino en orden.
def camino(grafo_canciones_usuarios, origen, destino, canciones, playlists):
    if origen not in canciones or destino not in canciones:
        print("Tanto el origen como el destino deben ser canciones", file = sys.stdout)
        return
    padre = camino_minimo_bfs(grafo_canciones_usuarios, origen, destino)
    if destino not in padre:
        print("No se encontro recorrido", file = sys.stdout)
        return
    imprimir_desde_origen(padre[destino], destino, padre, canciones, grafo_canciones_usuarios, playlists, 0)
    print("", file = sys.stdout)
    return

def imprimir_desde_origen(paso1, paso2, padre, canciones, grafo_canciones_usuarios, playlists, iteracion):
    if paso1 == None:
        print(paso2, end='', file = sys.stdout)
        return
    imprimir_desde_origen(padre[paso1], paso1, padre, canciones, grafo_canciones_usuarios, playlists, iteracion+1)
    playlist = playlists[grafo_canciones_usuarios.peso(paso1, paso2)]    
    if iteracion % 2 == 0: # paso2 es cancion y paso1, usuario.
        print(" --> tiene una playlist -->",playlist,"--> donde aparece -->",paso2, end='', file = sys.stdout)
    else: # paso2 es usuario y paso1, cancion.
        print(" --> aparece en playlist -->",playlist,"--> de -->",paso2, end='', file = sys.stdout)

def camino_minimo_bfs(grafo,origen,destino):
    padre = {}
    visitado = set()
    padre[origen] = None
    q = deque()
    q.append(origen)
    visitado.add(origen)
    while q:
        v = q.popleft()
        for w in grafo.adyacentes(v):
            if w not in visitado:
                padre[w] = v
                visitado.add(w)
                q.append(w)
                if w == destino:
                    return padre
    return padre

def pagerank(grafo, canciones, d=0.85, i = 25):
    pageranks = {}
    for v in grafo.lista_vertices():
        pageranks[v] = [1, grafo.adyacentes(v)]
    for _ in range(i):
        for v in pageranks:
            actualizar_pagerank(v, d, pageranks)
    lista_en_orden = []
    for cancion in canciones:
        lista_en_orden.append((cancion, pageranks[cancion][0]))
    lista_en_orden.sort(key = lambda x: x[1])
    lista_en_orden.reverse()
    return lista_en_orden

def actualizar_pagerank(v, d, pageranks):
    sumatoria = 0
    for adyacente in pageranks[v][1]:
        cant_adyacentes = len(pageranks[adyacente][1])
        sumatoria += pageranks[adyacente][0]/cant_adyacentes
    pageranks[v][0] = (1-d)/len(pageranks) + d*sumatoria

# Busca n canciones mas importantes y las imprime.
def mas_importantes(n, lista, cantidad):
    if n > cantidad:
        n = cantidad
    if n < 2:
        if n == 1:
            print(lista[1], file = sys.stdout)
            return
        print("Numero de parametro invalido", file = sys.stdout)
        return
    lista_reducida = lista[:n]
    for i in range(n-1):
        a_imprimir = lista_reducida[i][0]+"; "
        print(a_imprimir, end='', file = sys.stdout)
    print(lista_reducida[n-1][0], file = sys.stdout)
    return

# Devuelve n canciones o usuarios recomendados.
def recomendacion(grafo, tipo, n, canciones_base, canciones, usuarios, iteraciones=5, largo=20):
    vertices = []
    if tipo == 'canciones':
        for cancion in canciones_base:
            vertices.append(cancion)
    else:
        for cancion in canciones_base:
            adyacentes = grafo.adyacentes(cancion)
            vertices.append(adyacentes[randint(0, len(adyacentes)-1)])
    lista_en_orden = pagerank_personalizado(grafo, vertices, iteraciones, largo, tipo, canciones_base, canciones, usuarios)
    lista_reducida = lista_en_orden[:n]
    for i in range(n-1):
        a_imprimir = lista_reducida[i][0]+"; "
        print(a_imprimir, end='', file = sys.stdout)
    print(lista_reducida[n-1][0], file = sys.stdout)
    return

def pagerank_personalizado(grafo, vertices, iteraciones, largo, tipo, canciones_base, canciones, usuarios):
    pageranks = {}
    lista_en_orden = []
    for v in grafo.lista_vertices():
        pageranks[v] = 1
    for _ in range(iteraciones):
        for v in vertices:
            random_walk(grafo, v, 0, 1, largo, pageranks)
    if tipo == 'canciones':
        for cancion in canciones:
            if cancion not in canciones_base:
                lista_en_orden.append((cancion, pageranks[cancion]))
    else:
        for usuario in usuarios:
            lista_en_orden.append((usuario, pageranks[usuario]))
    lista_en_orden.sort(key = lambda x: x[1])
    lista_en_orden.reverse()
    return lista_en_orden

def random_walk(grafo, vertice, iteracion, transferido, largo, pageranks):
    if iteracion >= largo:
        return
    if iteracion > 0:
        pageranks[vertice] += transferido
    adyacentes = grafo.adyacentes(vertice)
    adyacente_random = adyacentes[randint(0, len(adyacentes)-1)]
    nuevo_adyacentes = grafo.adyacentes(adyacente_random)
    nuevo_vertice = nuevo_adyacentes[randint(0, len(nuevo_adyacentes)-1)]
    transferido = transferido/len(adyacentes)
    random_walk(grafo, nuevo_vertice, iteracion+1, transferido, largo, pageranks)

# Obtiene un ciclo de largo n.
def ciclo(grafo, n, cancion):
    if n < 2:
        if n == 1:
            print(cancion, file = sys.stdout)
            return
        print("Numero de parametro invalido", file = sys.stdout)
        return
    visitados = set()
    camino = []
    if ciclo_hamiltoniano_dfs(grafo, cancion, visitados, camino, n):
        for c in camino:
            print(c,"--> ", end='', file = sys.stdout)
        print(cancion, file = sys.stdout)
        return
    print("No se encontro recorrido", file = sys.stdout)
    return

def ciclo_hamiltoniano_dfs(grafo, cancion, visitados, camino, n):
    camino.append(cancion)
    visitados.add(cancion)
    if len(camino) > n:
        visitados.remove(cancion)
        camino.pop()
        return False
    for w in grafo.adyacentes(cancion):
        if w == camino[0]:
            if len(camino) == n:
                return True
        if w not in visitados:
            if ciclo_hamiltoniano_dfs(grafo, w, visitados, camino, n):
                return True
    visitados.remove(cancion)
    camino.pop()
    return False

# Muestra todas las canciones a exactamente n distancia.
def rango(grafo_canciones_canciones, n, cancion, canciones):
    if n < 0:
        print("Numero de parametro invalido", file = sys.stdout)
        return
    if cancion not in canciones:
        print("El origen debe ser cancion", file = sys.stdout)
        return
    print(rango_bfs(grafo_canciones_canciones, cancion, n), file = sys.stdout)
    return

def rango_bfs(grafo, origen, n):
    resultado = 0
    distancia = {}
    visitado = set()
    distancia[origen] = 0
    q = deque()
    q.append(origen)
    visitado.add(origen)
    while q:
        v = q.popleft()
        for w in grafo.adyacentes(v):
            if w not in visitado:
                distancia[w] = distancia[v]+1
                if distancia[w] == n:
                    resultado += 1
                visitado.add(w)
                q.append(w)
    return resultado
