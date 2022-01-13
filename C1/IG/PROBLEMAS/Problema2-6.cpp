//Problema 2.6

#include <vector>

void CalculoArea(vector<Tupla3f> vertices, vector<Tupla3i> triangulos)
{
    float area_total=0.0;
    Tupla3f u, v;

    for(int i=0; i < triangulos.size(); i++){
        u=triangulos[i][1]-triangulos[i][0];
        v=triangulos[i][2]-triangulos[i][0];

        /*El valor módulo del producto vectorial de dos vectores equivale al área del paralelogramo que los tiene como lados.
        Como un paralelogramo se puede dividir en dos triángulos iguales que tiene ambos como catetos a los dos lados del paralelogramo tenemos que el área de cada triángulo de la tabla
        es la mitad del área del paralelogramo que tiene a u y v como lados.
        La función cross nos devuelve el producto vectorial de u por v. La función lengthSq nos devuelve la longitud al cuadrado de dicho producto vectorial y al hacer la raíz obtenemos
         el área del paralelogramo*/
        area += sqrt(u.cross(v).lengthSQ());
    }

    return area/2.0;
}