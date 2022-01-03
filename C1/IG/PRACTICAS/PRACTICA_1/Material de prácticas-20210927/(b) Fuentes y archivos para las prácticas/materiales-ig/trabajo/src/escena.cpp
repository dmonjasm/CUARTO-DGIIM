// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)

#include "ig-aux.h"
#include "escena.h"

#include "objeto3d.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "practicas.h"
#include "camara.h"
#include "materiales-luces.h"
#include "seleccion.h"
#include "modelo-jer.h"
#include "latapeones.h"




// -----------------------------------------------------------------------------------------------

Escena::Escena()
{
   // COMPLETAR: Práctica 4: inicializar 'col_fuentes' y 'material_ini'
   // ...
   col_fuentes = new Col2Fuentes();
   material_ini = new Material(0.2, 0.2, 0.3, 60);


   // COMPLETAR: Práctica 5: hacer 'push_back' de varias camaras perspectiva u ortogonales,
   // (sustituir la cámara orbital simple ('CamaraOrbitalSimple') por varias cámaras de 3 modos ('Camara3Modos')
   //camaras.push_back( new CamaraOrbitalSimple() );
   camaras.push_back( new Camara3Modos());
   camaras.push_back( new Camara3Modos(true, {6.0,6.0,6.0}, 1.0, {2.0,0.0,3.0}, 50.0));
   camaras.push_back( new Camara3Modos(false, {-6.0,6.0,6.0}, 1.0, {6.0,0.0,0.0}, 60.0));
   camaras.push_back( new Camara3Modos(true, {6.0,6.0,-6.0}, 1.0, {0.0,0.0,0.0}, 80.0));

}
// -----------------------------------------------------------------------------------------------
// visualiza la escena en la ventana actual, usando la configuración especificada en 'cv'

void Escena::visualizarGL( ContextoVis & cv )
{
   using namespace std ;

   // recuperar el cauce actual de 'cv' en 'cauce', activarlo
   Cauce * cauce = cv.cauce_act ; assert( cauce != nullptr );
   cauce->activar() ;

   // recuperar la cámara actual de esta escena y  fijar las matrices 'modelview'
   // y 'projection' en el cauce gráfico (es decir: activar la cámara actual)
   CamaraInteractiva * camara = camaras[ind_camara_actual] ; assert( camara != nullptr );
   const float ratio_vp = float(cv.ventana_tam_y)/float(cv.ventana_tam_x) ;
   camara->fijarRatioViewport( ratio_vp );
   camara->activar( *cauce ) ;


   // dibujar los ejes, si procede
   if ( cv.dibujar_ejes  )
      DibujarEjesSolido( *cauce ) ;

   // fijar el color por defecto en el cauce para dibujar los objetos
   // if ( cv.fondo_blanco && ! cv.iluminacion )
   //    glColor3f( 0.0, 0.0, 0.0 );
   // else
   //    glColor3f( 1.0, 1.0, 1.0 );
   glColor3f( 1.0, 1.0, 1.0 );

   // COMPLETAR: Práctica 1: Configurar el cauce en función de:
   cauce->fijarModoSombrPlano(cv.sombr_plano);

   //    cv.sombr_plano (true/false)              --> usar fijarModoSombrPlano (método del 'cauce')
   if(cv.modo_visu == ModosVisu::relleno){
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);  
   } 

   else if(cv.modo_visu == ModosVisu::lineas){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   }

   else if(cv.modo_visu == ModosVisu::puntos)
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   //    cv.modo_visu   (puntos,lineas o relleno) --> usar glPolygonMode


   if ( cv.iluminacion )
   {
      // COMPLETAR: Práctica 4: activar evaluación del MIL (y desactivar texturas)
      // * habilitar evaluación del MIL en el cauce (fijarEvalMIL)
      // * activar la colección de fuentes de la escena
      // * activar el material inicial
      // ....
      cv.cauce_act->fijarEvalMIL(true);
      col_fuentes->activar(*(cv.cauce_act));

      if(material_ini!=nullptr){
         cv.material_act = material_ini;
         material_ini->activar(cv);
      }

   }
   else // si la iluminación no está activada, deshabilitar MIL y texturas
   {  cauce->fijarEvalMIL( false );
      cauce->fijarEvalText( false );
   }

   // recuperar el objeto actual de esta escena
   Objeto3D * objeto = objetos[ind_objeto_actual] ; assert( objeto != nullptr );

   // COMPLETAR: Práctica 1: visualizar el objeto actual ('objeto')
   objeto->visualizarGL(cv);  
}





// -----------------------------------------------------------------------------------------------
// pasa la cámara actual a la siguiente

void Escena::siguienteCamara()
{
   assert( ind_camara_actual < camaras.size() );
   ind_camara_actual = (ind_camara_actual+1 ) % camaras.size();
   using namespace std ;
   cout << "Cámara actual cambiada a: " << (ind_camara_actual+1) << " (de " << camaras.size() << ")" << endl ;
}

// -----------------------------------------------------------------------------------------------
// pasa el objeto actual al siguiente

void Escena::siguienteObjeto()
{
   if ( objetos.size() == 0 )
      return ;
   assert( ind_objeto_actual < objetos.size() );
   ind_objeto_actual = (ind_objeto_actual+1 ) % objetos.size();
   using namespace std ;
   cout << "Objeto actual cambiado a: " << objetoActual()->leerNombre()
        << " (" << (ind_objeto_actual+1) << "/" << objetos.size() << ")." << endl  ;
}

// -----------------------------------------------------------------------------------------------
// devuelve puntero al objeto actual

Objeto3D * Escena::objetoActual()
{
   assert( ind_objeto_actual < objetos.size() );
   assert( objetos[ind_objeto_actual] != nullptr );
   return objetos[ind_objeto_actual] ;
}
// -----------------------------------------------------------------------------------------------
// devuelve un puntero a la cámara actual

CamaraInteractiva * Escena::camaraActual()
{
   assert( ind_camara_actual < camaras.size() );
   assert( camaras[ind_camara_actual] != nullptr );
   return camaras[ind_camara_actual] ;
}
// -----------------------------------------------------------------------------------------------
// devuelve un puntero a la colección de fuentes actual

ColFuentesLuz * Escena::colFuentes()
{
   assert( col_fuentes != nullptr );
   return col_fuentes ;
}
// -----------------------------------------------------------------------------------------------

Escena1::Escena1()
{
   using namespace std ;
   cout << "Creando objetos de escena 1 .... " << flush ;

   //Añado primer objeto del examen
   objetos.push_back( new Piramide(0.5,1.0,0.8));

   // añadir el objeto 'Cubo' a la lista de objetos de esta escena:
   objetos.push_back( new Cubo() );

   // COMPLETAR: Práctica 1: creación del resto objetos de la práctica 1
   // Añadir objetos al vector 'objetos', con:
   //     objetos.push_back( new .... )
   // .........
   objetos.push_back( new EstrellaZ(10));
   objetos.push_back( new CasaX());
   objetos.push_back( new MallaTriangulo());
   objetos.push_back( new MallaCuadrado());
   objetos.push_back( new MallaPiramideL());
   objetos.push_back( new CuboTejado() );
   objetos.push_back( new Tetraedro() );
   objetos.push_back( new CuboColores() );

   cout << "hecho." << endl << flush ;
}

// -------------------------------------------------------------------------
// COMPLETAR: Práctica 2
// Añadir la implementación del constructor de la clase Escena2 para construir
// los objetos que se indican en los guiones de las práctica 2
// .......

Escena2::Escena2()
{
   using namespace std;
   cout << "Creando objetos de escena 2 .... " << flush ;

   objetos.push_back( new ConoExamen(16,1.0,0.8,1.0));
   objetos.push_back( new PiramideEstrellaZ(8));
   objetos.push_back( new RejillaY(7,8));
   objetos.push_back( new MallaTorre(5));
   objetos.push_back( new MallaPLY("../recursos/plys/ant.ply"));
   objetos.push_back( new MallaPLY("../recursos/plys/beethoven.ply"));
   objetos.push_back( new MallaPLY("../recursos/plys/big_dodge.ply"));
   objetos.push_back( new MallaRevolPLY("../recursos/plys/lata_pcue.ply",20));
   objetos.push_back( new MallaRevolPLY("../recursos/plys/lata_pinf.ply",20));
   objetos.push_back( new MallaRevolPLY("../recursos/plys/lata_psup.ply",20));
   objetos.push_back( new MallaRevolPLY("../recursos/plys/peon.ply",20));
   objetos.push_back( new Cilindro(20,20));
   objetos.push_back( new Cono(20,20));
   objetos.push_back( new Esfera(20,20));

   cout << "hecho." << endl << flush;
}



// -------------------------------------------------------------------------
// COMPLETAR: Práctica 3
// Añadir la implementación del constructor de la clase Escena3 para construir
// los objetos que se indican en los guiones de las práctica 3
// .......
Escena3::Escena3()
{
   using namespace std ;
   cout << "Creando objetos de escena 3 .... " << flush ;

   objetos.push_back(new VariosCubos(8,0.9,0.8,1.0));
   objetos.push_back(new NodoRaiz());
   objetos.push_back(new GrafoEstrellaX(10));
   objetos.push_back(new GrafoCubos());

   cout << "hecho." << endl << flush ;
}



// ----------------------------------------------------------------------
// COMPLETAR: Práctica 4
// Añadir la implementación del constructor de la clase Escena4 para construir
// los objetos que se indican en los guiones de las práctica 4
// .......
Escena4::Escena4()
{
   using namespace std;

   cout << "Creando objetos de escena 4 .... " << flush;

   objetos.push_back(new NodoCubo());
   objetos.push_back(new LataPeones());
   objetos.push_back(new NodoDiscoP4());

   cout << "hecho." << endl << flush;
}


// ----------------------------------------------------------------------
// COMPLETAR: Práctica 5
// Añadir la implementación del constructor de la clase Escena5 para construir
// los objetos que se indican en los guiones de las práctica 5
// .......

Escena5::Escena5()
{
   using namespace std;

   cout << "Creando objetos de escena 5 .... " << flush;

   objetos.push_back(new LataPeones_def());
   
   cout << "hecho." << endl << flush;

}



