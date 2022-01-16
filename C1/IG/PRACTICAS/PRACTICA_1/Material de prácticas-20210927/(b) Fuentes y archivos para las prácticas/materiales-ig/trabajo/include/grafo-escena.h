// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Gestión de una grafo de escena (declaraciones)
// ** Copyright (C) 2014-2017 Carlos Ureña
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


#ifndef GRAFO_ESCENA_HPP
#define GRAFO_ESCENA_HPP

//#include <obj-gra.hpp>
//#include <materiales.hpp>

#include "matrices-tr.h"
#include "objeto3d.h"
#include "materiales-luces.h"

// *********************************************************************
// declaración adelantada de estructura para un nodo del grafo de escena

class NodoGrafoEscena ;
class GrafoEstrellaX;

// *********************************************************************
// tipo enumerado con los tipos de entradas del nodo del grafo de escena

enum class TipoEntNGE { objeto, transformacion, material, noInicializado } ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

struct EntradaNGE
{
   TipoEntNGE tipo ;   // objeto, transformacion, material
   union
   {  Objeto3D * objeto ;  // ptr. a un objeto (no propietario)
      Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
      Material * material ; // ptr. a material (no propietario)
   } ;
   // constructores (uno por tipo)
   EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
   EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
   EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
   ~EntradaNGE() ;
} ;

// *********************************************************************
// Nodo del grafo de escena: es un objeto 3D parametrizado, que contiene una lista de entradas

class NodoGrafoEscena : public Objeto3D
{
   protected:
   // COMPLETAR: práctica 3: definir variables y métodos privados del nodo
   // .......
   std::vector<EntradaNGE> entradas;
   bool centro_calculado = false;
   
   public:

   NodoGrafoEscena() ;

   // visualiza usando OpenGL
   virtual void visualizarGL( ContextoVis & cv ) ;

   // añadir una entrada al final, hace copia de la entrada
   // devuelve indice de la entrada dentro del vector de entradas
   unsigned agregar( const EntradaNGE & entrada );

   // construir una entrada y añadirla (al final)
   unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
   unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
   unsigned agregar( Material * pMaterial ); // material (copia solo puntero)

   // devuelve el puntero a la matriz en la i-ésima entrada
   Matriz4f * leerPtrMatriz( unsigned iEnt );

   // método para buscar un objeto con un identificador
   virtual bool buscarObjeto( const int ident_busc, const Matriz4f & mmodelado,
                    Objeto3D ** objeto, Tupla3f & centro_wc )  ;

   // si 'centro_calculado' es 'false', recalcula el centro usando los centros
   // de los hijos (el punto medio de la caja englobante de los centros de hijos)
   virtual void calcularCentroOC() ;




} ;

class GrafoEstrellaX : public NodoGrafoEscena
{
   protected:
      Matriz4f * rotacion_objeto=nullptr;

      void actualizarEstadoParametro(const unsigned int iParam, const float t_sec);
   public:
      GrafoEstrellaX(unsigned n);

      void fijar_rotacion_objeto(const float nuevarot);

      unsigned leerNumParametros() const;

};

class ConoDesplazado : public NodoGrafoEscena
{
   public:
      ConoDesplazado();
};

class EstrellaXDesplazada : public NodoGrafoEscena
{
   public:
      EstrellaXDesplazada(unsigned n);
};

class GrafoCubos : public NodoGrafoEscena
{
   protected:
      Matriz4f * rotacion_cubos1=nullptr,
               * rotacion_cubos2=nullptr,
               * rotacion_cubos3=nullptr,
               * rotacion_cubos4=nullptr,
               * rotacion_cubos5=nullptr,
               * rotacion_cubos6=nullptr;

      void actualizarEstadoParametro(const unsigned int iParam, const float t_sec);
   
   public:
      GrafoCubos();

      void fijar_rotacion_objeto(const float nuevarot);

      unsigned leerNumParametros() const;

};

class CuboCentral : public NodoGrafoEscena
{
   public: 
      CuboCentral();
};

class CarasParalelas : public NodoGrafoEscena
{
   public:
      CarasParalelas();
};

class PrismaGiratorio : public NodoGrafoEscena
{
   public:
      PrismaGiratorio(Matriz4f * & rotacion_prisma);
};

class CuboEscalado : public NodoGrafoEscena
{
   public:
      CuboEscalado(float escala, Matriz4f * & matriz_escalado);
};


class VariosCubos : public NodoGrafoEscena
{
   protected:
      Matriz4f * escalado_cubos=nullptr;
      float cota_inf=0.0, cota_sup=0.0, periodo=0.0;

      void actualizarEstadoParametro(const unsigned int iParam, const float t_sec);

   public:
      VariosCubos(int n, float s_min, float s_max, float T);

      void fijar_escalado_cubos(const float nuevo_escalado);

      unsigned leerNumParametros() const;
};

class NodoDiscoP4 : public NodoGrafoEscena
{
   public:  
      NodoDiscoP4();
};

class GrafoEscenasP5 : public NodoGrafoEscena
{
   public:
      GrafoEscenasP5();
};

class GrafoEscenasP5_2 : public NodoGrafoEscena
{
   public:
      GrafoEscenasP5_2();
};

class P4Nodo : public NodoGrafoEscena
{
   public:
      P4Nodo();

};

class P5Nodo : public NodoGrafoEscena
{
   public:
      P5Nodo();
};

#endif // GRAFO_ESCENA_HPP

// *********************************************************************
