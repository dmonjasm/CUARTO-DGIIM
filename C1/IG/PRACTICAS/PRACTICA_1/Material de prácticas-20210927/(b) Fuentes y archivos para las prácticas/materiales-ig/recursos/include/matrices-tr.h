// *********************************************************************
// **
// ** Gestión de matrices 4x4 de transformacion (declaraciones)
// ** Copyright (C) 2014 Carlos Ureña
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


#ifndef MATRICES_TR_HPP
#define MATRICES_TR_HPP

#include <vector>
#include "matrizg.h"

// ---------------------------------------------------------------------
// juego de tests de la matrices
void MAT_Tests() ;

// ---------------------------------------------------------------------
// creación y operadores de matrices: transformaciones de modelado

Matriz4f MAT_Ident( ) ;
Matriz4f MAT_Traslacion( const float d[3] ) ;
Matriz4f MAT_Traslacion( const float dx, const float dy , const float dz ) ;

Matriz4f MAT_Escalado( const float sx, const float sy, const float sz ) ;
Matriz4f MAT_Rotacion( const float ang_gra, const float ex, const float ey, const float ez ) ;
Matriz4f MAT_Rotacion( const float ang_gra, const Tupla3f & eje ) ;
Matriz4f MAT_Filas( const Tupla3f & fila0, const Tupla3f & fila1, const Tupla3f & fila2 );

// ---------------------------------------------------------------------
// matrices auxiliares para la transformación de vista

Matriz4f MAT_Transpuesta3x3( const Matriz4f & org ) ;
Matriz4f MAT_Filas         ( const Tupla3f fila[3] );
Matriz4f MAT_Columnas      ( const Tupla3f colum[3] );

// ---------------------------------------------------------------------
// matrices para la transformacion de vista:

Matriz4f MAT_LookAt( const float origen[3], const float centro[3], const float vup[3] );
Matriz4f MAT_Vista         ( const Tupla3f eje[3], const Tupla3f& org );
Matriz4f MAT_Vista_inv     ( const Tupla3f eje[3], const Tupla3f& org );


// ---------------------------------------------------------------------
// matrices de transformación del viewport

Matriz4f MAT_Viewport      ( int org_x, int org_y, int ancho, int alto );
Matriz4f MAT_Viewport_inv  ( int org_x, int org_y, int ancho, int alto );

// ---------------------------------------------------------------------  
// matrices para la transf. de proyección

Matriz4f MAT_Frustum    ( const float l, const float r, const float b, const float t, const float n, const float f );
Matriz4f MAT_Ortografica( const float l, const float r, const float b, const float t, const float n, const float f );
Matriz4f MAT_Perspectiva( const float fovy_grad, const float raz_asp, const float n, const float f );

// --------------------------------------------------------------------
// calcula la inversa (la última fila debe ser 0, 0, 0, 1 - es decir,
// no debe haber proyeccion)
Matriz4f MAT_Inversa( const Matriz4f & m );

// -------------------------------------------------------------------
// una clase para una pila de matrices

class PilaMatriz4f
{
   private:
      std::vector<Matriz4f> anteriores ;

   public:
      Matriz4f actual  ;
      // crea una pila, donde la matriz actual es la identidad
      inline PilaMatriz4f()
      {  actual = MAT_Ident() ;
      }
      // crear una pila, dándole un valor a la matriz actual
      inline PilaMatriz4f( const Matriz4f & actual_ini )
      {  actual = actual_ini ;
      }
      // hacer push de la pila (guarda la actual)
      inline void push(  )
      {  anteriores.push_back( actual );
      }
      // componer una matriz
      inline void comp( const Matriz4f & mat )
      {  actual = actual*mat ;
      }
      // hacer pop de la pila (recupera la última guardada con push)
      inline void pop()
      {  const size_t n = anteriores.size() ;
         //assert( 0 < n );
         actual = anteriores[n-1] ;
         anteriores.pop_back() ;
      }
} ;




#endif
