// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
#ifndef MODELO_JER_H
#define MODELO_JER_H

#include "grafo-escena.h"

class NodoRaiz : public NodoGrafoEscena
{
    protected:
        Matriz4f * pm_trasl_cajon1 = nullptr;
        Matriz4f * pm_trasl_cajon2 = nullptr;
        Matriz4f * pm_trasl_cajon3 = nullptr;
        Matriz4f * pm_rot_comoda = nullptr;
                 
        void actualizarEstadoParametro( const unsigned iPparam, const float t_sec);         
                

    public:
        NodoRaiz();

        unsigned leerNumParametros() const;

        void fijar_pm_trasl_cajon1(const float nuevatrasl_cajon1);
        void fijar_pm_trasl_cajon2(const float nuevatrasl_cajon2);
        void fijar_pm_trasl_cajon3(const float nuevatrasl_cajon3);

        void fijar_pm_rot_comoda(const float nuevarot_comoda);

};

class PrismaAchatado : public NodoGrafoEscena
{   
    public:
        PrismaAchatado(const float ancho, const float alto,const float largo);
};

class CuerpoComoda : public NodoGrafoEscena
{
    public:
        CuerpoComoda();
};

class TapasLaterales : public NodoGrafoEscena
{
    public:
        TapasLaterales(const float ancho, const float alto, const float largo, const float distancia);
};

class TapasVerticales : public NodoGrafoEscena
{
    public:
        TapasVerticales(const float ancho, const float alto, const float largo, const float distancia);
};


class TapaPosteriorDelantera : public NodoGrafoEscena
{
    public:
        TapaPosteriorDelantera(const float ancho, const float alto, const float largo, const float lejania);
};

class Cajon : public NodoGrafoEscena
{
    public:
        Cajon(Matriz4f * & traslacion);
};

class TresCajones : public NodoGrafoEscena
{
    public:
        TresCajones(Matriz4f * & traslacion1,Matriz4f * & traslacion2,Matriz4f * & traslacion3);
};

class Pata: public NodoGrafoEscena
{
    public:
        Pata();
};

class CuatroPatas : public NodoGrafoEscena
{
    public:
        CuatroPatas();
};

class ComodaCompleta : public NodoGrafoEscena
{
    public:
        ComodaCompleta(Matriz4f * & traslacion1,Matriz4f * & traslacion2,Matriz4f * & traslacion3, Matriz4f * & rotacion);
};



#endif