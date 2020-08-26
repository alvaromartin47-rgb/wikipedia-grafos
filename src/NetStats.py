#!/usr/bin/python3

#########################################################################################
                                                                                        #
                                    # NETSTATS                                          #
                                                                                        #
######################################################################################### 

import sys
from mod.mensajes import *
from mod.comandos import FUNCIONALIDADES, funcionalidad
from lib.operaciones import cargar_contenido
                                                                                     
# Esta constante almacena el directorio python path para incluir archivos y carpetas    
# en el programa.                                                                       
                                                                                        
PATH = "."                                                                          
sys.path.append(PATH)                                                                   
                                                                                        
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
    if len(parametros) not in range(n_params[0], n_params[1]):
        print(ERR_PARAMS.format(comando))

    funcionalidad(red_internet, comando, *parametros)


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
