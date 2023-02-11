#!/usr/bin/python3
from grafo import *
from funciones import *
import sys

# Se ejecuta el programa con:
# python3 tp3.py archivo.tsv

def procesar_archivo(ruta_archivo):
    grafo_canciones_usuarios = Grafo()
    canciones = set()
    usuarios = {}
    playlists = {}
    with open(ruta_archivo) as f:
        next(f)
        for linea in f:
            _, nombre_usuario, nombre_cancion, artista, id_playlist, nombre_playlist, _ = linea.rstrip('\n').split('	')
            cancion_completa = nombre_cancion+" - "+artista
            grafo_canciones_usuarios.agregar_vertice(cancion_completa)
            grafo_canciones_usuarios.agregar_vertice(nombre_usuario)
            grafo_canciones_usuarios.agregar_arista(cancion_completa, nombre_usuario, id_playlist)
            canciones.add(cancion_completa)
            playlists[id_playlist] = nombre_playlist
            if nombre_usuario not in usuarios:
                usuarios[nombre_usuario] = [id_playlist]
            else:
                usuarios[nombre_usuario].append(id_playlist)
    return grafo_canciones_usuarios, canciones, usuarios, playlists

def crear_grafo_canciones(grafo_canciones_usuarios, canciones):
    grafo_canciones_canciones = Grafo()
    for cancion in canciones:
        grafo_canciones_canciones.agregar_vertice(cancion)
        for creador in grafo_canciones_usuarios.adyacentes(cancion):
            for otra_cancion in grafo_canciones_usuarios.adyacentes(creador):
                grafo_canciones_canciones.agregar_arista(cancion, otra_cancion, creador)
    return grafo_canciones_canciones

def main():
    if len(sys.argv) != 2:
        print("Cantidad de parametros incorrectos", file = sys.stdout)
        return
    grafo_canciones_usuarios, canciones, usuarios, playlists = procesar_archivo(sys.argv[1])
    grafo_canciones_canciones = None
    lista_importantes = pagerank(grafo_canciones_usuarios, canciones)
    for linea in sys.stdin:
        linea = linea.rstrip('\n')
        if linea[:6] == 'camino':
            origen, destino = linea[7:].split(' >>>> ')
            camino(grafo_canciones_usuarios, origen, destino, canciones, playlists)
        elif linea[:5] == 'ciclo':
            if grafo_canciones_canciones == None:
                grafo_canciones_canciones = crear_grafo_canciones(grafo_canciones_usuarios, canciones)
            linea = (linea[6:].split(' '))
            n = linea.pop(0)
            cancion = ' '.join(linea)
            ciclo(grafo_canciones_canciones, int(n), cancion)
        elif linea[:5] == 'rango':
            if grafo_canciones_canciones == None:
                grafo_canciones_canciones = crear_grafo_canciones(grafo_canciones_usuarios, canciones)
            linea = (linea[6:].split(' '))
            n = linea.pop(0)
            cancion = ' '.join(linea)
            rango(grafo_canciones_canciones, int(n), cancion, canciones)
        elif linea[:15] == 'mas_importantes':
            n = linea[16:]
            mas_importantes(int(n), lista_importantes, len(canciones))
        elif linea[:13] == 'recomendacion':
            linea = (linea[14:].split(' '))
            tipo = linea.pop(0)
            n = linea.pop(0)
            canciones_base = ' '.join(linea)
            canciones_base = canciones_base.split(' >>>> ')
            recomendacion(grafo_canciones_usuarios, tipo, int(n), canciones_base, canciones, usuarios)

main()

