// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
#include "ig-aux.h"
#include "tuplasg.h"
#include "practicas.h"
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"

// framebuffer object usado para selección
static Framebuffer * fbo = nullptr ;

// ---------------------------------------------------------------------

Framebuffer * FBOActual()
{
   return fbo ;
}

// ---------------------------------------------------------------------
// fijar el color actual en el cauce usando un identificador entero

void FijarColVertsIdent( Cauce & cauce, const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
   // .....
   const unsigned char
      byteR = ( ident             ) % 0x100U,
      byteG = ( ident /    0x100U ) % 0x100U,
      byteB = ( ident /  0x10000U ) % 0x100U;

      glColor3ub(byteR, byteG, byteB);
}

// ----------------------------------------------------------------------------------
// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....(sustituir el 'return 0' por lo que corresponda)
   // .....
   unsigned char bytes[3];

   glReadPixels( xpix, ypix, 1,1, GL_RGB, GL_UNSIGNED_BYTE, (void *)bytes);

   return bytes[0] + ( 0x100U*bytes[1] ) + ( 0x10000U*bytes[2] );
}

// -------------------------------------------------------------------------------
// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
//
// devuelve: true si se ha seleccionado algún objeto, false si no

bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib )
{
   using namespace std ;
   assert( escena != nullptr );

   // COMPLETAR: práctica 5:
   // Visualizar escena en modo selección y leer el color del pixel en (x,y)
   // Se deben de dar estos pasos:

   //cout << "Seleccion( x == " << x << ", y == " << y << ", obj.raíz ==  " << objeto_raiz.leerNombre() << " )" << endl ;

   // 1. Crear (si es necesario) y activar el framebuffer object (fbo) de selección
   // .........
   if(fbo == nullptr){
      fbo = new Framebuffer(cv_dib.ventana_tam_x,cv_dib.ventana_tam_y);
   }

   fbo->activar(cv_dib.ventana_tam_x,cv_dib.ventana_tam_y);

   // 2. crear un 'ContextoVis' apropiado, en ese objeto:
   //    * activar modo selecion, desactivar iluminación, poner modo relleno
   //    * usar el mismo cauce, y la misma cámara que en 'cv_dib'
   //    * fijar el tamaño de la ventana igual que en 'cv_dib'
   //
   // ..........

   ContextoVis cv_aux(cv_dib);

   cv_aux.modo_seleccion = true;
   cv_aux.iluminacion = false;
   cv_aux.modo_visu = ModosVisu::relleno;

   FijarColVertsIdent(*cv_aux.cauce_act, 0);


   // 3. Activar fbo, cauce y viewport. Configurar cauce (modo solido relleno, sin ilum.
   //    ni texturas). Limpiar el FBO (color de fondo: 0)
   // .......
   fbo->activar(cv_aux.ventana_tam_x,cv_aux.ventana_tam_y);//Activo el fbo

   cv_aux.cauce_act->activar();//Activo el cauce
   cv_aux.cauce_act->fijarEvalMIL(false);//Desactivo la iluminación
   cv_aux.cauce_act->fijarEvalText(false);//Desactivo las texturas
   cv_aux.cauce_act->fijarModoSombrPlano(true);

   glViewport(0,0, cv_aux.ventana_tam_x, cv_aux.ventana_tam_y);//Fijo la matriz del viewport

   glClearColor(0,0,0,1);//Limpio el color y lo pongo a 0 de color de fondo
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Limpia colores y Z-búffer


   // 4. Activar la cámara (se debe leer de la escena con 'camaraActual')
   // ....
   CamaraInteractiva * camara_actual = escena->camaraActual();

   camara_actual->activar(*cv_aux.cauce_act);


   // 5. Visualizar el objeto raiz actual (se debe leer de la escena con 'objetoActual()')
   // ........
   Objeto3D * objeto_actual = escena->objetoActual();

   objeto_actual->visualizarGL(cv_aux);


   // 6. Leer el color del pixel (usar 'LeerIdentEnPixel')
   // (hay que hacerlo mientras está activado el framebuffer de selección)
   // .....
   int id_pixel = LeerIdentEnPixel(x,y);

   // 7. Desactivar el framebuffer de selección
   // .....
   fbo->desactivar();


   // 8. Si el identificador del pixel es 0, imprimir mensaje y terminar (devolver 'false')
   // ....
   if(id_pixel == 0){
      cout << "Identificado igual a 0; no se realiza ninguna acción" << endl;
      return false;
   }



   // 9. Buscar el objeto en el objeto_raiz (puede ser un grafo de escena)
   //    e informar del nombre del mismo (si no se encuentra, indicarlo)
   //   (usar 'buscarObjeto')
   // .....
   Objeto3D * auxiliar;
   Tupla3f centro;

   if(objeto_actual->buscarObjeto(id_pixel, MAT_Ident(), &auxiliar, centro)){
      camara_actual->mirarHacia(centro);
      cout << "Objeto seleccionado\nIdentificador: " << id_pixel << "\nNombre: " << auxiliar->leerNombre() << endl;
   }

   else{
      cout << "No se ha seleccionado un objeto, aunque el identificador sea mayor que 0" << endl;
      return false;
   }

   // al final devolvemos 'true', ya que hemos encontrado un objeto
   return true ;
}
