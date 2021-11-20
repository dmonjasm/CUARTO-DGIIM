// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"
#include "matrices-tr.h"
#include "grafo-escena.h"
#include <cmath>

NodoRaiz::NodoRaiz(){
    ponerNombre("Comoda Ikea");
    ponerIdentificador(-1);

    agregar(new ComodaCompleta(pm_trasl_cajon1,pm_trasl_cajon2,pm_trasl_cajon3,pm_rot_comoda));
};

void NodoRaiz::actualizarEstadoParametro(const unsigned int iParam, const float t_sec){
    assert(iParam < leerNumParametros());

    switch(iParam){
        case 0:
            fijar_pm_trasl_cajon1(1.0/3.0*(1.0 + sin(M_PI*t_sec)));
        break;

        case 1:
            fijar_pm_trasl_cajon2(2.0/3.0*(1.0 + sin(M_PI*t_sec)));
        break;

        case 2:
            fijar_pm_trasl_cajon3(1.0 + sin(M_PI*t_sec));
        break;

        case 3:
            fijar_pm_rot_comoda(30 * t_sec);
        break;
    }
};

unsigned NodoRaiz::leerNumParametros() const
{
    return 4;
};

void NodoRaiz::fijar_pm_trasl_cajon1(const float nuevatrasl_cajon1)
{
    *pm_trasl_cajon1 = MAT_Traslacion(0.0,0.0,nuevatrasl_cajon1);
};

void NodoRaiz::fijar_pm_trasl_cajon2(const float nuevatrasl_cajon2)
{
    *pm_trasl_cajon2 = MAT_Traslacion(0.0,0.0,nuevatrasl_cajon2);
};

void NodoRaiz::fijar_pm_trasl_cajon3(const float nuevatrasl_cajon3)
{
    *pm_trasl_cajon3 = MAT_Traslacion(0.0,0.0,nuevatrasl_cajon3);
};

void NodoRaiz::fijar_pm_rot_comoda(const float nuevarot_comoda)
{
    *pm_rot_comoda = MAT_Rotacion(nuevarot_comoda, 0.0, 1.0, 0.0);
}

PrismaAchatado::PrismaAchatado(const float ancho, const float alto, const float largo){
    ponerIdentificador(-1);

    agregar(MAT_Escalado(ancho,alto,largo));
    agregar(new Cubo());

    if(ancho == (float)0.7)
        ponerColor({0.15,0.15,0.15});        
};

TapasVerticales::TapasVerticales(const float ancho, const float alto, const float largo, const float distancia)
{
    ponerIdentificador(-1);

    agregar( MAT_Traslacion(0.0,-distancia/2-alto/2,0.0));
    agregar( new PrismaAchatado(ancho/2,alto/2,largo/2));
    agregar( MAT_Traslacion(0.0, distancia+alto, 0.0));
    agregar( new PrismaAchatado(ancho/2,alto/2,largo/2));

    ponerColor({0.552,0.286,0.145});
};

TapasLaterales::TapasLaterales(const float ancho, const float alto, const float largo, const float distancia)
{
    ponerIdentificador(-1);

    agregar( MAT_Traslacion(-distancia/2-ancho/2,0.0,0.0));
    agregar( new PrismaAchatado(ancho/2,alto/2,largo/2));
    agregar( MAT_Traslacion(distancia+ancho, 0.0, 0.0));
    agregar( new PrismaAchatado(ancho/2,alto/2,largo/2));

    ponerColor({0.305,0.231,0.192});
};

TapaPosteriorDelantera::TapaPosteriorDelantera(const float ancho, const float alto, const float largo, const float lejania)
{
    ponerIdentificador(-1);

    agregar(MAT_Traslacion(0.0,0.0,lejania));
    agregar( new PrismaAchatado(ancho/2,alto/2,largo/2));

    ponerColor({0.501,0.25,0.0});
};

CuerpoComoda::CuerpoComoda()
{
    ponerIdentificador(-1);

    agregar(new TapasVerticales(1.5,0.1,2.0,1.8));
    agregar(new TapasVerticales(1.5,0.1,2.0,1.6/3));
    agregar(new TapasLaterales(0.1,2.0,2.0,1.5));
    agregar(new TapaPosteriorDelantera(1.7,2.0,0.1,-1.05));
    
};

Cajon::Cajon(Matriz4f * & traslacion)
{   
    ponerIdentificador(-1);

    unsigned ind = agregar(MAT_Traslacion(0.0,0.0,0.0));
    PrismaAchatado* fondo_cajon;
    
    agregar(new TapasLaterales(0.05,1.6/3.0,1.95,1.4));
    agregar(new TapaPosteriorDelantera(1.6,1.6/3.0+0.1,0.05,1.0));
    agregar(new TapaPosteriorDelantera(1.5,1.6/3.0,0.05,-1.0));
    agregar(MAT_Traslacion(0.0,-1.6/6+0.025,0.0));
    agregar(new PrismaAchatado(0.7,0.025,0.975));
    agregar(MAT_Traslacion(0.0,1.6/6.0-0.025,1.075));
    agregar(MAT_Escalado(0.1,0.1,0.1));
    agregar(new Esfera(20,20));

    traslacion = leerPtrMatriz(ind);
    

}

TresCajones::TresCajones(Matriz4f * & trasl1,Matriz4f * & trasl2,Matriz4f * & trasl3)
{
    ponerIdentificador(-1);

    agregar(new Cajon(trasl2));
    agregar(MAT_Traslacion(0.0,-1.6/3-0.1,0.0));
    agregar(new Cajon(trasl3));
    agregar(MAT_Traslacion(0.0,2*1.6/3.0+0.2,0.0));
    agregar(new Cajon(trasl1));

};

Pata::Pata()
{
    ponerIdentificador(-1);

    agregar(MAT_Traslacion(0.75,-1.2,0.85));
    agregar(MAT_Escalado(0.15,0.1,0.15));
    agregar(new Esfera(30,30));    
}

CuatroPatas::CuatroPatas()
{
    ponerIdentificador(-1);

    agregar(new Pata());
    agregar(MAT_Escalado(-1.0,1.0,-1.0));
    agregar(new Pata());
    agregar(MAT_Rotacion(-82.5,0.0,1.0,0.0));
    agregar(new Pata());
    agregar(MAT_Escalado(-1.0,1.0,-1.0));
    agregar(new Pata());
};

ComodaCompleta::ComodaCompleta(Matriz4f * & traslacion1,Matriz4f * & traslacion2,Matriz4f * & traslacion3, Matriz4f * & rotacion)
{
    ponerIdentificador(-1);

    unsigned ind = agregar( MAT_Rotacion(0.0,0.0,1.0,0.0));

    agregar(new CuatroPatas());
    agregar(new TapasVerticales(1.9,0.15,2.2,2.0));
    agregar(new CuerpoComoda());
    agregar(new TresCajones(traslacion1,traslacion2,traslacion3));
    

    rotacion = leerPtrMatriz(ind);
};
