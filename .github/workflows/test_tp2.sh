#!/bin/bash

cd tp2
echo ""
echo "Descomprimiendo pruebas catedra..."
unzip pruebas.zip > /dev/null

cd ..
echo ""
echo "Compilando tp2..."
make tp2_t

cd tp2/pruebas_zyxcba
echo ""
echo "Ejecutando pruebas..."
./pruebas.sh ./pruebas_tp2

cd ..
echo ""
echo "Eliminando archivos y carpetas innecesarios..."
rm -r pruebas_zyxcba
