// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

//#include <set>   // std::set
#include "ig-aux.h"
#include "tuplasg.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"


// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....

   Tupla3f ver1, ver2, ver3, aux1, aux2, normal;
   for(unsigned int i = 0; i<triangulos.size(); i++){
      ver1 = vertices[triangulos[i](0)];
      ver2 = vertices[triangulos[i](1)];
      ver3 = vertices[triangulos[i](2)];

      aux1 = ver2-ver1;
      aux2 = ver3-ver1;

      normal = aux1.cross(aux2);
      if(normal.lengthSq() > 0)
         nor_tri.push_back(normal.normalized());
      else
         nor_tri.push_back({0.0, 0.0, 0.0});  
   }

}


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   
   // Calculamos las normales de cada triangulo
   calcularNormalesTriangulos();

   // Inicializamos vector
   for (unsigned int i = 0; i < vertices.size(); ++i){
      nor_ver.push_back({0.0, 0.0, 0.0});
   }

   // A cada vertice de un triangulo le sumamos la normal de dicho triangulo. No hace falta dividir porque se normaliza
   for (unsigned int i = 0; i < triangulos.size(); ++i){
      nor_ver[triangulos[i](0)] = nor_ver[triangulos[i](0)] + nor_tri[i];
      nor_ver[triangulos[i](1)] = nor_ver[triangulos[i](1)] + nor_tri[i];
      nor_ver[triangulos[i](2)] = nor_ver[triangulos[i](2)] + nor_tri[i];
   }

   // Guardamos la normal normalizada de cada verice si no es nula
   for (unsigned int i = 0; i < vertices.size(); ++i){
      if (nor_ver[i].lengthSq() > 0)
         nor_ver[i] = nor_ver[i].normalized();
   }

}




// ----------------------------------------------------------------------------


void MallaInd::visualizarGL( ContextoVis & cv )
{


   using namespace std ;

   assert( cv.cauce_act != nullptr );

   if ( cv.visualizando_normales )
   {  visualizarNormales(  );
      return ;
   }

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   // guardar el color previamente fijado
   const Tupla4f color_previo = leerFijarColVertsCauce( cv ); //Se usa en practica 5

   // COMPLETAR: práctica 1: si el puntero 'array_verts' es nulo, crear el objeto ArrayVerts
   //   * en el constructor se dan los datos de la tabla de coordenadas de vértices (tabla 'vertices')
   //   * después hay que invocar a 'fijarIndices', usando el formato y datos de la tabla de triángulos ('triangulos')
   //   * si las tablas 'col_ver', 'cc_tt_ver' o 'nor_ver' no están vacías, hay que invocar los métodos 
   //     'fijarColores', 'fijarCoordText' y 'fijarNormales', como corresponda.

   if(array_verts == nullptr){
      array_verts = new ArrayVertices(GL_FLOAT, 3, vertices.size(), vertices.data());
   

      array_verts->fijarIndices(GL_UNSIGNED_INT, triangulos.size()*3, triangulos.data());

      if(!col_ver.empty()){
         array_verts->fijarColores(GL_FLOAT, 3, col_ver.data() );
      }

      if(!cc_tt_ver.empty()){
         array_verts->fijarCoordText(GL_FLOAT, 2, cc_tt_ver.data() );
      }

      if(!nor_ver.empty()){
         array_verts->fijarNormales(GL_FLOAT, nor_ver.data() );
      }
   }




   // COMPLETAR: práctica 1: visualizar según el modo (en 'cv.modo_envio')
   //   ** inmediato begin/end       : usar método 'visualizarGL_MI_BVE' de 'ArrayVerts'
   //   ** inmediato con una llamada : usar método 'visualizarGL_MI_DAE' de 'ArrayVerts'
   //   ** diferido (con un VAO)     : usar método 'visualizarGL_MD_VAO' de 'ArrayVerts'
   // (en cualquier caso hay que pasar como parámetro el tipo de primitiva adecuada a una malla de triángulos).
   // .....
   if(cv.modo_envio == ModosEnvio::inmediato_begin_end){
      array_verts->visualizarGL_MI_BVE(GL_TRIANGLES);
   }

   else if(cv.modo_envio == ModosEnvio::inmediato_drawelements){
      array_verts->visualizarGL_MI_DAE(GL_TRIANGLES);
   }

   else if(cv.modo_envio == ModosEnvio::diferido_vao){
      array_verts->visualizarGL_MD_VAO(GL_TRIANGLES);
   }


   // restaurar el color previamente fijado
   glColor4fv( color_previo );
}

void MallaInd::visualizarNormales(){
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  
   if ( array_verts_normales == nullptr )
   {  
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {  
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      array_verts_normales = new ArrayVertices( GL_FLOAT, 3, segmentos_normales.size(), segmentos_normales.data() );
   }

   array_verts_normales->visualizarGL_MI_DAE( GL_LINES );
   CError();
}

// *****************************************************************************


// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................
   LeerPLY(nombre_arch, vertices, triangulos);



   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();


}

// ****************************************************************************
// Clase 'Cubo
Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

   calcularNormales();

}



// ****************************************************************************
// Clase 'CuboTejado'

CuboTejado::CuboTejado()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +0.0, +2.0, +0.0 }, // 8
      } ;

   for( Tupla3f & v : vertices )
      v = 2.0f*v +Tupla3f({0.0,2.0,0.0});



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         {2,3,8}, {3,7,8}, {7,6,8}, {6,2,8}, // añado tejado 

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}
// -----------------------------------------------------------------------------------------------

Tetraedro::Tetraedro()
:  MallaInd( "tetraedro 4 vértices")
{
   vertices = 
   { { -1.0, -0.575, -0.815 }, // 0
      { 1.0, -0.575, -0.815 }, // 1
      { +0.0, +1.155, -0.815 }, // 2
      { +0.0, +0.0, +0.815 }, // 3
   };

   ponerColor({1.0,0.0,1.0});

   triangulos = 
   {
      {0,1,2}, {0,1,3}, {1,2,3}, {0,2,3}
   };

   calcularNormales();
}
//-------------------------------------------------------------------------------------------------------
CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices coloreado" )
{
   float R=0.0, G=0.0, B=0.0;
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;

   for(unsigned int i = 0; i < vertices.size(); i++){
      if(vertices[i][0] == -1.0){
         R=-1.0;
      }

      else if(vertices[i][0] == +1.0){
         R=+1.0;
      }

      if(vertices[i][1] == -1.0){
         G=-1.0;
      }

      else if(vertices[i][1] == +1.0){
         G=+1.0;
      }

      if(vertices[i][2] == -1.0){
         B=-1.0;
      }

      else if(vertices[i][2] == +1.0){
         B=+1.0;
      }

      col_ver.push_back({R,G,B});
   }



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

};

EstrellaZ::EstrellaZ(unsigned n)
{
   assert(n > 1);

   ponerNombre("Ejercicio Adicional Practica 1:EstrellaZ");

   //Inserto el centro
   vertices.push_back({0.5,0.5,0.0});
   //Le pongo color blanco
   col_ver.push_back({1.0,1.0,1.0});

   Tupla3f largo, corto;
   largo={0.5,0.0,0.0};
   corto={0.15,0.0,0.0};

   corto=MAT_Rotacion((float)(360/(2*n)),0.0,0.0,1.0)*corto;

   for(int i=0; i < n; i++){
      vertices.push_back(MAT_Traslacion(0.5,0.5,0.0)*MAT_Rotacion((float)(360/n)*i,0.0,0.0,1.0)*largo);
      vertices.push_back(MAT_Traslacion(0.5,0.5,0.0)*MAT_Rotacion((float)(360/n)*i,0.0,0.0,1.0)*corto);
   }

   for(int i=1; i < 2*n+1; i++){
      col_ver.push_back(vertices[i]);//No se inculye el primer vertice que ya tiene color, y se tienen que tener en cuenta los 2n restantes
   }

   for(int i=0; i < 2*n-1; i++){
      triangulos.push_back({0,i+1,i+2});
   }

   triangulos.push_back({0,1,2*n}); //Inserto el último triángulo

};

CasaX::CasaX()
{
   ponerNombre("Ejercicio Adicional Practica 1: CasaX");

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +1.0, +1+sin(45)*0.5/cos(45), +0.0}, //Punto X+ del tejado 8
         { -1.0, +1+sin(45)*0.5/cos(45), 0.0} //Punto X- del tejado 9
      } ;

      for(int i=0; i < vertices.size(); i++){
         vertices[i]=MAT_Escalado(1.2,1.0,1.0)*vertices[i];
      }

      for(int i=0; i < 10; i++)
         col_ver.push_back(vertices[i]);



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         //{0,5,1}, {0,4,5}, // Y- Quito los triángulos de la base
         //{2,3,7}, {2,7,6}, // Y+ (+2) Quito los triángulos de la tapa


         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3},  // Z+ (+1)

         //Tejado Casa
         {8,6,7}, {6,8,2}, {8,2,9},
         {3,9,8}, {3,7,8}, {3,2,9}
      } ;
};

MallaTriangulo::MallaTriangulo()
{
   ponerNombre("Ejercicio Adicional Practica 1:Malla Triangulo");

   vertices = 
      {  {-0.5,-sqrt(2)+sqrt(10)/4.0,0.0},
         {0.0,sqrt(10)/4.0,0.0},
         {0.5,-sqrt(2)+sqrt(10)/4.0,0.0},
      };

   triangulos={{0,1,2}};
};

MallaCuadrado::MallaCuadrado()
{
   ponerNombre("Ejercicio Adicional Practica 1:Malla Cuadrado");

   vertices =
      {  {-1.0,-1.0,0.0}, //0
         {-1.0, 1.0,0.0}, //1
         {1.0,-1.0,0.0}, //2
         {1.0,1.0, 0.0} //3
      };  

   triangulos =
      {
         {0,1,2}, {1,2,3}
      };
}

MallaPiramideL::MallaPiramideL()
{  
   ponerNombre("Ejercicio Adicional Practica 1:PiramideL");

   vertices={
      {-1.0,-1.0,0.0}, //0
      {1.0,-1.0,0.0}, //1
      {1.0,0.0,0.0}, //2
      {0.0,0.0,0.0}, //3
      {0.0,2.0,0.0}, //4
      {-1.0,2.0,0.0}, //5
      {0.0,0.0,1.0} //6
   };  

   for(int i=0; i < vertices.size(); i++){
      col_ver.push_back(vertices[i]);
   }

   triangulos={
      {0,1,3}, {1,2,3}, {3,4,5}, {5,0,3},
      {5,4,6}, {1,2,6}, {6,3,2}, {6,3,4},
      {0,5,6}, {0,1,6}
   };

}

PiramideEstrellaZ::PiramideEstrellaZ(unsigned n)
{
   assert(n>1);

   ponerNombre("Ejercicio Adicional Practica 2: PiramideEstrellaZ");

   //Inserto el centro
   vertices.push_back({0.5,0.5,0.0});
   //Le pongo color blanco
   col_ver.push_back({1.0,1.0,1.0});

   Tupla3f largo, corto;
   largo={0.5,0.0,0.0};
   corto={0.15,0.0,0.0};

   corto=MAT_Rotacion(22.5,0.0,0.0,1.0)*corto;

   for(int i=0; i < n; i++){
      vertices.push_back(MAT_Traslacion(0.5,0.5,0.0)*MAT_Rotacion(45*i,0.0,0.0,1.0)*largo);
      vertices.push_back(MAT_Traslacion(0.5,0.5,0.0)*MAT_Rotacion(45*i,0.0,0.0,1.0)*corto);
   }

   for(int i=1; i < 2*n+1; i++){
      col_ver.push_back(vertices[i]);//No se inculye el primer vertice que ya tiene color, y se tienen que tener en cuenta los 2n restantes
   }

   for(int i=0; i < 2*n-1; i++){
      triangulos.push_back({0,i+1,i+2});
   }

   triangulos.push_back({0,1,2*n}); //Inserto el último triángulo

   //Hasta aquí tenemos la base
   vertices.push_back({0.5,0.5,0.5});
   col_ver.push_back({1.0,1.0,1.0});

   for(int i=1; i < 2*n+1; i++){
      triangulos.push_back({0,i,2*n+1});
   }

   
}

RejillaY::RejillaY(unsigned m, unsigned n)
{
   ponerNombre("Ejercicio Adicional Practica 2: RejillaY");

   assert(n>1 && m>1);
   Tupla3f centro={0.0,0.0,0.0};

   float ancho_x=(float)1.0/(m-1);
   float largo_z=(float)1.0/(n-1);

   for(int i=0; i < n; i++){
      for(int j=0; j < m; j++){
         vertices.push_back(MAT_Traslacion(j*ancho_x,0.0,i*largo_z)*centro);
      }
   }

   for(int i=0; i < vertices.size();i++)
      col_ver.push_back(vertices[i]);

   for(int i=0; i < n-1; i++){
      for(int j=0; j < m-1; j++){
         triangulos.push_back({j+i*m,j+(i+1)*m,i*m+j+1}); 
         triangulos.push_back({j+(i+1)*m,i*m+j+1,j+(i+1)*m+1}); 
      }
   }   
}

MallaTorre::MallaTorre(int n)
{
   assert(n >= 1);

   

   vertices.clear();
   ponerNombre("Ejercicio Adicional Práctica 2: MallaTorre");

   Tupla3f punto_inicial = {-0.5,0.0,0.5};

   for(int i=0; i < n+1; i++){
      for(int j=0; j < 4;j++){
         vertices.push_back(MAT_Traslacion(0.0,i*1.0,0.0)*MAT_Rotacion(j*90,0.0,1.0,0.0)*punto_inicial);
      }
   }

   for(int i=0; i < vertices.size(); i++)
      col_ver.push_back(vertices[i]);

   for(int i=0; i < n; i++){
      for(int j=0; j < 4; j++){
         if(j!=3){
            triangulos.push_back({j+i*4,(j+1+i*4),(i+1)*4+j});
            triangulos.push_back({j+1+i*4,(i+1)*4+j,(i+1)*4+j+1});
         }
         
         else{
            triangulos.push_back({j+i*4,(j+1+i*4)-4,(i+1)*4+j}); 
            triangulos.push_back({j+1+i*4-4,(i+1)*4+j,(i+1)*4+j+1-4});
         }

      }
   }

}

Piramide::Piramide(float s, float t, float h)
{
   ponerNombre("Piramide Examen");

   assert(s < t);

   vertices={
         {t/2,0.0,t/2}, //0
         {t/2,0.0,-t/2}, //1
         {-t/2,0.0,-t/2}, //2
         {-t/2,0.0,t/2}, //3
         {s/2,h,s/2}, //4
         {s/2,h,-s/2}, //5
         {-s/2,h,-s/2}, //6
         {-s/2,h,s/2} //7
   };

   triangulos={
      {0,4,5}, {5,1,0},
      {1,5,6}, {1,2,6},
      {6,7,2}, {2,7,3},
      {7,3,4}, {4,3,0}

   };
}

ConoExamen::ConoExamen(int n, float r_1, float r_2, float h)
{
   assert(r_2 < r_1);

   ponerNombre("Cono Examen");

   Tupla3f inicial_bajo={r_1,0.0,0.0};
   Tupla3f inicial_alta={r_2,h,0.0};

   vertices.push_back(inicial_bajo);
   vertices.push_back(inicial_alta);

   for(int i=1; i < n; i++){
      vertices.push_back(MAT_Rotacion((float)360/(n-1)*i,0.0,1.0,0.0)*vertices[0]);
      vertices.push_back(MAT_Rotacion((float)360/(n-1)*i,0.0,1.0,0.0)*vertices[1]);
   }

   for(int i=0; i < 2*n; i++){
      triangulos.push_back({i,(i+1)%(2*n),(i+2)%(2*n)});
   }
}
   
Cubo24 :: Cubo24(){
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

	 { -1.0, -1.0, -1.0 }, // 0 +8
         { -1.0, -1.0, +1.0 }, // 1 +8
         { -1.0, +1.0, -1.0 }, // 2 +8
         { -1.0, +1.0, +1.0 }, // 3 +8
         { +1.0, -1.0, -1.0 }, // 4 +8
         { +1.0, -1.0, +1.0 }, // 5 +8
         { +1.0, +1.0, -1.0 }, // 6 +8
         { +1.0, +1.0, +1.0 }, // 7 +8

	 { -1.0, -1.0, -1.0 }, // 0 +16
         { -1.0, -1.0, +1.0 }, // 1 +16
         { -1.0, +1.0, -1.0 }, // 2 +16 
         { -1.0, +1.0, +1.0 }, // 3 +16
         { +1.0, -1.0, -1.0 }, // 4 +16
         { +1.0, -1.0, +1.0 }, // 5 +16
         { +1.0, +1.0, -1.0 }, // 6 +16
         { +1.0, +1.0, +1.0 } // 7 +16
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)
         {8,13,9}, {8,12,13}, // Y-
         {10,11,15}, {10,15,14}, // Y+ (+2)
         {16,22,20}, {16,18,22}, // Z-
         {17,21,23}, {17,23,19}  // Z+ (+1)
      } ;

   cc_tt_ver=
     {{0,1}, // 0
      {1,1}, // 1
      {0,0}, // 2
      {1,0}, // 3
      {1,1}, // 4
      {0,1}, // 5
      {1,0}, // 6
      {0,0}, // 7

      {0,0}, // 0 +8
      {1,0}, // 1 +8
      {1,0}, // 2 +8
      {0,0}, // 3 +8
      {0,1}, // 4 +8
      {1,1}, // 5 +8
      {1,1}, // 6 +8
      {0,1}, // 7 +8

      {1,1}, // 0 +16
      {0,1}, // 1 +16
      {1,0}, // 2 +16 
      {0,0}, // 3 +16
      {0,1}, // 4 +16
      {1,1}, // 5 +16
      {0,0}, // 6 +16
      {1,0} // 7 +16
     };

   calcularNormales();
}

NodoCubo :: NodoCubo(){
   ponerNombre("Cubo 24 vertices");
   
   Textura * tex = new Textura("../recursos/imgs/window-icon.jpg");
   
   agregar( new Material(tex, 0.2, 0.4, 0.4, 20) );
   agregar(new Cubo24());


}

MallaDiscoP4::MallaDiscoP4()
{
   ponerColor({1.0,1.0,1.0}); 
   const unsigned ni = 23, nj = 31;
   Tupla2f aux_tex={0.0,0.0};
   cc_tt_ver.clear();

   for( unsigned i=0; i < ni; i++)
   for( unsigned j=0; j < nj; j++)
   {
      const float
         fi = float(i)/float(ni-1),
         fj = float(j)/float(nj-1),
         ai = 2.0*M_PI*fi,
         x = fj * cos( ai ),
         y = fj * sin( ai ),
         z = 0.0;

         //Ejercicio Adicional 1 P4
         aux_tex(0)=(x+1)/2.0;
         aux_tex(1)=(y+1)/2.0;
      
      vertices.push_back({x,y,z});

      //Se inserta la coordenada de textura tras ajustarla al plano [0,1]^2
      //Ejercicio Adicional 1 P4
      cc_tt_ver.push_back(aux_tex);

      //Ejercicio adicional 2 P4
      //cc_tt_ver.push_back({fi, fj});

   }

   for(unsigned i=0; i < ni-1; i++)
   for(unsigned j=0; j < nj-1; j++)
   {
      triangulos.push_back({i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1)});
      triangulos.push_back({i*nj+j, (i+1)*nj+(j+1),(i+1)*nj+j});
   }

   calcularNormales();
}

P4Malla::P4Malla()
{
   ponerNombre("Malla examen práctica 4 (21-22)");
    
   vertices =
   {   { 0.0, 1.0, 0.0 }, { +1.0, 0.0, +1.0  }, { +1.0, 0.0, -1.0 },  // mira hacia X+ e Y+
      { 0.0, 1.0, 0.0 }, { -1.0, 0.0, -1.0  }, { -1.0, 0.0, +1.0 },  // mira hacia X- e Y+
      { 0.0, 1.0, 0.0 }, { -1.0, 0.0, +1.0  }, { +1.0, 0.0, +1.0 },  // mira hacia Z+ e Y+
      { 0.0, 1.0, 0.0 }, { +1.0, 0.0, -1.0  }, { -1.0, 0.0, -1.0 }   // mira hacia Z- e Y+
   } ;

   triangulos =
   {  {0,1,2}, {3,4,5}, {6,7,8}, {9,10,11}
   } ;

   cc_tt_ver=
   {
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
      {0.5,1.0}, {0.0,0.0}, {1.0,0.0},
   };

   nor_ver={
      (vertices[0] - (vertices[4]-vertices[5])/2).normalized(), {0.0,0.0,1.0}, {0.0,0.0,-1.0},
      (vertices[0] - (vertices[1]-vertices[2])/2).normalized(), {0.0,0.0,-1.0}, {0.0,0.0,1.0},
      (vertices[0] - (vertices[10]-vertices[11])/2).normalized(), {-1.0,0.0,0.0}, {1.0,0.0,0.0},
      (vertices[0] - (vertices[7]-vertices[8])/2).normalized(), {1.0,0.0,0.0}, {-1.0,0.0,0.0}
   };
}

P5Malla::P5Malla(){
   ponerIdentificador(-1);

   ponerNombre("P5Malla");

   vertices = 
   {  {1.0,1.0,1.0}, {1.0,-1.0,1.0},
      {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}
   };

   triangulos =
   {
      {0,1,2}, {0,2,3}
   };
}


