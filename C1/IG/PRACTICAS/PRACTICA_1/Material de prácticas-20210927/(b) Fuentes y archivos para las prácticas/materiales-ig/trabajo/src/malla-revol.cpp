// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "tuplasg.h"
#include "lector-ply.h"
#include "matrices-tr.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 2: completar: creación de la malla....
   Tupla3f eje = Tupla3f(0.0,1.0,0.0);
   int m=perfil.size();

   if(vertices.empty()){

      for(int i=0; i <= num_copias-1; i++){
         for(int j=0; j <= m-1; j++){
            float angulo=(360*i)/(num_copias-1);
            Matriz4f rotacion= MAT_Rotacion(angulo,eje);

            vertices.push_back(rotacion*perfil[j]);            
         }
      }

      int k=0;

      for(int i=0; i <= num_copias-2; i++){
         for(int j=0; j <= m-2; j++){
            k=i*m + j;

            triangulos.push_back(Tupla3i(k,k+m,k+m+1));
            triangulos.push_back(Tupla3i(k,k+m+1,k+1));
         }
      }
   }
}

// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);

}

// -----------------------------------------------------------------------------
// constructor, cilindro"
Cilindro::Cilindro(const int num_verts_per,const unsigned nperfiles)
{
   ponerNombre( std::string("Cilindro"));
   vector<Tupla3f> perfil;
   if(num_verts_per < 4){
      std::cerr<< "Numero de vértices del perfil del cilindro insuficiente" << endl;
   }

   perfil.push_back({0.0,0.0,0.0});

   for(int i=0; i <= num_verts_per-3; i++){
      perfil.push_back({1.0,(float)i/(num_verts_per-3),0.0});
   }

   perfil.push_back({0.0,1.0,0.0});
   
   inicializar(perfil, nperfiles);
}


// -----------------------------------------------------------------------------
// constructor, cono"
Cono::Cono(const int num_verts_per, const unsigned nperfiles)
{
   ponerNombre( std::string("Cono"));

   vector<Tupla3f> perfil;
   Tupla3f eje={0.0,1.0,0.0};


   perfil.push_back({0.0,0.0,0.0});
   float y=0.0;
   float x=0.0;

   for(int i=0; i <= num_verts_per-1; i++){
      y=(float)i/(num_verts_per-1);
      x=1-y;

      perfil.push_back({x,y,0.0});
   }

   inicializar(perfil, nperfiles);
}
// -----------------------------------------------------------------------------
// constructor, esfera"


Esfera::Esfera(const int num_verts_per, const unsigned nperfiles)
{
   ponerNombre( std::string("Esfera"));

   vector<Tupla3f> perfil;
   Tupla3f eje={0.0,0.0,1.0};
   float angulo=0.0;
   perfil.push_back({0.0,-1.0,0.0});

   for(int i=0; i <= num_verts_per-1; i++){
      angulo=(float)180*i/(num_verts_per-1);

      perfil.push_back(MAT_Rotacion(angulo, eje)*perfil[0]);
   }

   inicializar(perfil, nperfiles);

   ponerColor({1.0,1.0,0.0});
   
}


