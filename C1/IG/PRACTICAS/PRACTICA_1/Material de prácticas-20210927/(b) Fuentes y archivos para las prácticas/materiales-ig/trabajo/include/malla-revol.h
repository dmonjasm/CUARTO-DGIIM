// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "malla-ind.h"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para mallas indexadas obtenidas a partir de la revolución de un perfil

class MallaRevol : public MallaInd
{
   private:



   protected: //

   MallaRevol() {} // solo usable desde clases derivadas con constructores especificos

   // Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
   // a partir de un perfil y el número de copias que queremos de dicho perfil.
   void inicializar
   (
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned               num_copias  // número de copias del perfil
   ) ;
} ;
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class MallaRevolPLY : public MallaRevol
{
   public:
   MallaRevolPLY( const std::string & nombre_arch,
                  const unsigned nperfiles ) ;
} ;
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class Cilindro : public MallaRevol
{
   public:

   Cilindro(
      const int num_verts_per,
      const unsigned nperfiles
   );

};
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class Cono : public MallaRevol
{  
   public:

   Cono(
      const int num_Verts_per,
      const unsigned nperfiles
   );

};
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class Esfera : public MallaRevol
{
   public:

   Esfera(
      const int num_verts_per,
      const unsigned nperfiles
   );

};


#endif
