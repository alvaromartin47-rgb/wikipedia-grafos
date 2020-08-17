#########################################################################################
                                                                                        #
                                # FUNCIONES IMPRIMIR                                    #
                                                                                        #
######################################################################################### 

def imprimir_form_flecha(solucion):
    if solucion != []: print(solucion[0], end="")
    
    for i in range(1, len(solucion)):
        print(f" -> {solucion[i]}", end="")
    
    print()

def imprimir_form_coma(solucion):
    print(solucion[0], end="")
    for i in range(1, len(solucion)): print(f", {solucion[i]}", end="")
    print()

def imprimir_componente(componente):
    imprimir_form_coma(componente)

def imprimir_lectura(orden, primera):
    solucion = list()

    while True:
        solucion.append(primera)
        if not orden[primera]: break
        primera = orden[primera]
    
    imprimir_form_coma(solucion)

def imprimir_ciclo(ciclo, destino):
    solucion = list()

    while True:
        solucion.insert(0, destino)
        
        if ciclo[destino] == solucion[len(solucion) - 1]:
            solucion.insert(0, ciclo[destino])
            break
        
        destino = ciclo[destino]

    imprimir_form_flecha(solucion)

def imprimir_camino(padres, destino, costo=None):
    solucion = list()
    costo_aux = 0

    while True:
        solucion.insert(0, destino)
        if not padres[destino]: break
        costo_aux += 1
        destino = padres[destino]
    
    imprimir_form_flecha(solucion)

    if not costo: print(f"Costo: {costo_aux}")
    else: print(f"Costo: {costo}")