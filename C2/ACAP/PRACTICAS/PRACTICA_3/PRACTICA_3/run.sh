echo "Sincronizando..."
./sync.sh
echo "Conectando..."
ssh estudiante20@genmagic.ugr.es \
	"cd PRACTICA_3 && echo 'Compilando...' && nvcc pr3.cu -o pr3 && echo 'Ejecutando...' && ./pr3_trans"
