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
    
    cant_parametros = FUNCIONALIDADES.get(comando)
    if str(len(parametros)) != cant_parametros and cant_parametros != "n":
         print(ERR_PARAMS.format(comando))

    elif comando == "listar_comandos": listar_comandos(FUNCIONALIDADES)
    elif comando == "camino": camino(red_internet, parametros[0], parametros[1])
    elif comando == "conectados": conectados(red_internet, parametros[0])
    elif comando == "ciclo": ciclo()
    elif comando == "lectura": lectura()
    elif comando == "mas_importantes": mas_importantes()
    # Poner elif por cada comando


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