#!/bin/bash

cd ../tp2
echo ""
echo "Descomprimiendo pruebas catedra..."
unzip pruebas.zip > /dev/null
cd ../testing_scripts

echo ""
echo "Creando Makefile..."
cp ../tp2/deps.mk .
python3 worker.py -f deps.mk
rm -rf deps.mk
mv Makefile ../tp2

cd ../tp2
echo ""
echo "Compilando tp2..."
make zyxcba
mv zyxcba pruebas_zyxcba

cd pruebas_zyxcba
echo ""
echo "Ejecutando pruebas..."
./pruebas.sh ./zyxcba

cd ..
echo ""
echo "Eliminando archivos y carpetas innecesarios..."
rm -r pruebas_zyxcba
make clean
rm -rf Makefile
