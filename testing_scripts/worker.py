import argparse

CFLAGS = "-g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic"
VFLAGS = "--leak-check=full --track-origins=yes --show-reachable=yes"

def leer_deps(ruta_archivo):
    '''Recibe una ruta de un archivo de dependencias make y
    devuelve un diccionario de reglas de la forma {"ejecutable":["dep1", "dep2", etc...]}
    '''

    reglas_objetos = {}
    reglas_ejec = {}

    with open(ruta_archivo) as deps:
        for linea in deps:
            linea = linea.split(":")
            ejecutable = linea[0]
            reglas_ejec[ejecutable] = []
            
            objetos = linea[1].split()
            for o in objetos:
                parse_objeto = o.split(".")

                if parse_objeto[len(parse_objeto) - 1] != "o":
                    return {}, {}
                
                reglas_ejec[ejecutable].append(o)
                
                parse_c_file = []
                for i in range(len(parse_objeto) - 1):
                    parse_c_file.append(parse_objeto[i])
                
                parse_c_file.append("c")
                c_file = ".".join(parse_c_file)
                reglas_objetos[o] = c_file

            reglas_ejec[ejecutable] = " ".join(reglas_ejec[ejecutable])
            
    
    return reglas_ejec, reglas_objetos


def crear_makefile(reglas_ejec, reglas_objetos):
    '''Recibe dos diccionarios de reglas de make y crea un
    Makefile de compilacion de dichos archivos para gcc.
    '''
    
    with open("Makefile", "w") as archivo:
        archivo.write("# CREADO POR ALVARO MARTIN EN PYTHON :)\n")
        archivo.write(f"\nCFLAGS = {CFLAGS}\n")
        archivo.write(f"VFLAGS = {VFLAGS}\n")
        archivo.write(f"\n# OBJETOS A BORRAR\n")
        archivo.write(f"REMOVE =")

        for e in reglas_ejec:
            archivo.write(f" {reglas_ejec[e]}")
        archivo.write("\n")

        archivo.write("\n")
        archivo.write(f"\n# COMPILACION DE OBJETOS\n")
        for o in reglas_objetos:
            archivo.write(f"\n{o}: {reglas_objetos[o]}\n")
            archivo.write(f"{chr(9)}@- $(CC) -c $(CFLAGS) $^ -o $@\n")

        archivo.write("\n")
        archivo.write(f"\n# COMPILACION DE EJECUTABLES\n")
        for e in reglas_ejec:
            archivo.write(f"\n{e}: {reglas_ejec[e]}\n")
            archivo.write(f"{chr(9)}@- $(CC) $(CFLAGS) $^ -o $@\n")

        archivo.write("\n")
        archivo.write(f"\n# LIMPIAR\n")
        archivo.write(f"\nclean:\n")
        archivo.write(f"{chr(9)}@- rm -r $(REMOVE)\n")

        
def main():

    # Proceso entrada
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--file", help="Archivo deps.mk")
    args = parser.parse_args()
    parameters = vars(args)

    if args.file and parameters["file"].split(".")[1] == "mk":
        reglas_ejec, reglas_objetos = leer_deps(parameters["file"])
        if reglas_ejec == {}: print("ERROR: los objetos no estan bien definidos.")
        else: crear_makefile(reglas_ejec, reglas_objetos)
    
    else: print("ERROR: No se recibió el archivo de dependencias de make.")


main()