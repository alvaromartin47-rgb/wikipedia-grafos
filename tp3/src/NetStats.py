#########################################################################################
                                                                                        #
                                    # NETSTATS                                          #
                                                                                        #
######################################################################################### 

import sys
from mod.mensajes import *
from mod.comandos import *
from lib.operaciones import *
                                                                                     
# Esta constante almacena el directorio python path para incluir archivos y carpetas    
# en el programa.                                                                       
                                                                                        
PATH = "."                                                                          
sys.path.append(PATH)                                                                   
                                                                                        
# Esta constante contendra un diccionario que tiene como clave un comando y como valor  
# la cantidad de parámetros minimos para que pueda ejecutarse.                          
                                                                                        
FUNCIONALIDADES = obtener_comandos("mod/comandos.csv")

#########################################################################################
                                                                                        #
                                  # FLUJO DE PROGRAMA                                   #
                                                                                        #
######################################################################################### 

def procesar_comandos(comando, parametros, red_internet):
    if comando not in FUNCIONALIDADES: 
        print(ERR_CMD.format(comando))
        return
    
    n_params = FUNCIONALIDADES.get(comando)
    if n_params != "n" and len(parametros) not in range(n_params[0], n_params[1]):
        print(ERR_PARAMS.format(comando))

    if comando == "listar_operaciones": listar_comandos(FUNCIONALIDADES)
    elif comando == "camino": camino(red_internet, parametros[0], parametros[1])
    elif comando == "conectados": conectados(red_internet, parametros[0])
    elif comando == "ciclo": ciclo(red_internet, parametros[0], parametros[1])
    elif comando == "lectura": lectura(red_internet, parametros)
    elif comando == "mas_importantes": mas_importantes(red_internet, int(parametros[0]))
    elif comando == "diametro": diametro(red_internet)
    elif comando == "rango": todos_en_rango(red_internet, parametros[0], int(parametros[1]))
    elif comando == "navegacion": navegacion(red_internet, parametros[0])
    elif comando == "clustering": coeficiente_de_clustering(red_internet, parametros, len(parametros))
    elif comando == "comunidad": obtener_comunidad(red_internet, parametros[0])
    elif comando == "clear": clear()


def procesar_entrada(red_internet):
    for linea in sys.stdin:
        parse = linea.split()
        if len(parse) > 0:
            cmd = parse[0]
            parametros = []
            if len(parse) > 1:
                parametros = " ".join(parse[1:]).split(",")

            procesar_comandos(cmd, parametros, red_internet)

        else: print(ERR_FORMATO)
        

def main():
    
    if len(sys.argv) != 2:
        print(ERR_ARGV)
        return
    
    red_internet = cargar_contenido(sys.argv[1])
    
    procesar_entrada(red_internet)



if __name__ == "__main__":
    main()