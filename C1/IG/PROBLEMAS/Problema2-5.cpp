//Problema 2.5

#include <vector>

//Forma poco eficiente
vector<Tupla2i> CreacionTablaAristas(vector<Tupla3i> triangulos){
    vector<Tupla2i> ari;
    bool existe=false;

    /*Para cada tirangulo se comprueban una a una sus aristas, si ninguna de sus aristas (independientemente de su dirección), está en el vector de aristas
    entonces se inserta, si la arista (o la arista de dirección contraria), ya está, entonces esa arista no se inserta. Complejidad O(n)^2*/
    for(int i=0; i < triangulos.size(); i++){

        for(int j=0; j < triangulos.size() && !existe; j++){
            if(ari[i] == {triangulos[i][0],triangulos[i][1]} || ari[i] == {triangulos[i][1],triangulos[i][0]})
                existe=true;
        }

        if(!existe)
            ari.push_back({triangulos[i][0],triangulos[i][1]});

        existe=false;

        for(int j=0; j < triangulos.size() && !existe; j++){
            if(ari[i] == {triangulos[i][0],triangulos[i][2]} || ari[i] == {triangulos[i][2],triangulos[i][0]})
                existe=true;
        }

        if(!existe)
            ari.push_back({triangulos[i][0],triangulos[i][2]});

        existe=false;

        for(int j=0; j < triangulos.size() && !existe; j++){
            if(ari[i] == {triangulos[i][1],triangulos[i][2]} || ari[i] == {triangulos[i][2],triangulos[i][1]})
                existe=true;
        }

        if(!existe)
            ari.push_back({triangulos[i][1],triangulos[i][2]});

        existe=false;
    }

    return ari;
};


//Versión si los vértices están ordenados

void GenerarTablaAristas(vecto<Tupla3i> triangulos)
{
    vector<Tupla2i> ari;

    for(int i=0; i < triangulos.size(); i++){
        /*En cada triangulo de la tabla, se inserta las aristas (i,j), (j,k), (k,i) si y sólo si el primer termino es menor que el segundo*/
        for(int j=0; j < 3; j++){
            if(triangulo[i][j] < triangulo[i][(j+1)%3]){
                ari.push_back({triangulo[i][j],triangulo[i][(j+1)%3]});
            }
        }
    }
}






