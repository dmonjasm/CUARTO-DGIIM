// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "matrices-tr.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "objeto3d.h"
#include "malla-revol.h"
#include <cmath>

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........
   cv.cauce_act->pushMM();

   Tupla4f color_previo = leerFijarColVertsCauce( cv );

   for(unsigned i=0; i < entradas.size(); i++)
      switch(entradas[i].tipo)
      {  case TipoEntNGE::objeto:
            entradas[i].objeto->visualizarGL( cv );
            break;
         
         case TipoEntNGE::transformacion:
            cv.cauce_act->compMM( *(entradas[i].matriz));
            break;         
      }

   glColor4fv(color_previo);

   cv.cauce_act->popMM();
   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada des de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)



}




// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// ignora el colo o identificador del nodo, ignora las entradas de tipo material
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)




// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);
   return (entradas.size()-1); // sustituir por lo que corresponda ....

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)
   assert( indice < entradas.size() );
   assert( entradas[indice].tipo == TipoEntNGE::transformacion );
   assert( entradas[indice].matriz != nullptr );

   return entradas[indice].matriz;


}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)
   // ........


   // 2. si el identificador del nodo es el que se busca, ya está (terminar)
   // ........


   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')
   // ........


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}

ConoDesplazado::ConoDesplazado(){
   ponerIdentificador(-1);

   agregar(MAT_Traslacion(1.3,0.0,0.0));
   agregar(MAT_Rotacion(-90,0.0,0.0,1.0));
   agregar(MAT_Escalado(0.14,0.15,0.14));
   agregar(new Cono(20,20));
}

EstrellaXDesplazada::EstrellaXDesplazada(unsigned n){
   ponerIdentificador(-1);

   agregar(MAT_Escalado(2.6,2.6,1.0));
   agregar(MAT_Traslacion(-0.5,-0.5,0.0));
   agregar(new EstrellaZ(n));
}

GrafoEstrellaX::GrafoEstrellaX(unsigned n)
{
   ponerIdentificador(-1);
   ponerNombre("Ejercicio Adicional Practica 3: GrafoEstrellaX");

   unsigned ind = agregar(MAT_Rotacion(0,1.0,0.0,0.0));

   agregar(MAT_Rotacion(90,0.0,1.0,0.0));
   agregar(new EstrellaXDesplazada(n));
   for(int i=0; i < n; i++){
      agregar(MAT_Rotacion(360/n,0.0,0.0,1.0));
      agregar(new ConoDesplazado());
   }

   rotacion_objeto = leerPtrMatriz(ind);
}

unsigned GrafoEstrellaX::leerNumParametros() const
{
   return 1;
}

void GrafoEstrellaX::fijar_rotacion_objeto(const float nuevarot){
   *rotacion_objeto = MAT_Rotacion(nuevarot, 1.0,0.0,0.0);
}

void GrafoEstrellaX::actualizarEstadoParametro(const unsigned int iParam, const float t_sec)
{  
   assert(iParam < leerNumParametros());

   fijar_rotacion_objeto(900*t_sec);   
}

CarasParalelas::CarasParalelas()
{
   ponerIdentificador(-1);

   agregar(MAT_Traslacion(-0.5,-0.5,-0.5));
   agregar(new RejillaY(10,10));
   agregar(MAT_Traslacion(0.0,1.0,0.0));
   agregar(new RejillaY(10,10));
}

CuboCentral::CuboCentral()
{
   ponerIdentificador(-1);

   agregar(new CarasParalelas());
   agregar(MAT_Rotacion(90,0.0,0.0,1.0));
   agregar(new CarasParalelas());
   agregar(MAT_Rotacion(90,1.0,0.0,0.0));
   agregar(new CarasParalelas());
}

PrismaGiratorio::PrismaGiratorio(Matriz4f * & rotacion_prisma)
{

   unsigned int ind = agregar(MAT_Rotacion(0.0,1.0,0.0,0.0));

   agregar(MAT_Traslacion(0.75,0.0,0.0));
   agregar(MAT_Escalado(0.25,0.15,0.15));
   agregar(new Cubo());

   rotacion_prisma = leerPtrMatriz(ind);
}

GrafoCubos::GrafoCubos()
{
   ponerIdentificador(-1);

   agregar(new CuboCentral());
   agregar(new PrismaGiratorio(rotacion_cubos1));
   agregar(MAT_Rotacion(180,0.0,0.0,1.0));
   agregar(new PrismaGiratorio(rotacion_cubos2));
   agregar(MAT_Rotacion(90,0.0,1.0,0.0));
   agregar(new PrismaGiratorio(rotacion_cubos3));
   agregar(MAT_Rotacion(90,0.0,0.0,1.0));
   agregar(new PrismaGiratorio(rotacion_cubos4));
   agregar(MAT_Rotacion(90,0.0,0.0,1.0));
   agregar(new PrismaGiratorio(rotacion_cubos5));
   agregar(MAT_Rotacion(90,0.0,0.0,1.0));
   agregar(new PrismaGiratorio(rotacion_cubos6));
}

unsigned int GrafoCubos::leerNumParametros() const
{
   return 1;
}

void GrafoCubos::fijar_rotacion_objeto(const float nuevarot)
{
   *rotacion_cubos1 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
   *rotacion_cubos2 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
   *rotacion_cubos3 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
   *rotacion_cubos4 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
   *rotacion_cubos5 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
   *rotacion_cubos6 = MAT_Rotacion(nuevarot,1.0,0.0,0.0);
}

void GrafoCubos::actualizarEstadoParametro(const unsigned int iParam, const float t_sec)
{
   assert(iParam < leerNumParametros());

   fijar_rotacion_objeto(360*t_sec);
}

CuboEscalado::CuboEscalado(float escala, Matriz4f * & matriz_escalado){
   ponerIdentificador(-1);

   unsigned ind = agregar(MAT_Escalado(1.0,1.0,1.0));

   agregar(MAT_Escalado(escala,escala,escala));
   agregar(new Cubo());

   matriz_escalado=leerPtrMatriz(ind);
}

VariosCubos::VariosCubos(int n, float s_min, float s_max, float T){
   cota_sup=s_max;
   cota_inf=s_min;
   periodo=T;
   float s=(s_min+s_max)/2.0;

   CuboEscalado *instancia = new CuboEscalado(1.0, escalado_cubos);

   agregar(new Cubo());
   
   for(int i=1; i < n; i++){
      agregar(MAT_Traslacion(1.0+s,0.0,0.0));
      agregar(MAT_Escalado(s,s,s));
      agregar(instancia);
      //agregar(new CuboEscalado(pow(s,i), escalado_cubos));
   }
}

unsigned VariosCubos::leerNumParametros() const
{
   return 1;
}

void VariosCubos::fijar_escalado_cubos(const float nuevo_escalado)
{
   //*escalado_cubos=MAT_Escalado(nuevo_escalado,nuevo_escalado,nuevo_escalado);
}

void VariosCubos::actualizarEstadoParametro(const unsigned int iParam, const float t_sec)
{
   assert(iParam < leerNumParametros());

   fijar_escalado_cubos((cota_sup+cota_inf)/2.0+(cota_sup-cota_inf)/2.0*sin(2*M_PI*t_sec));
}






