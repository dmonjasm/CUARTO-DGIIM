// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)
#ifndef LATAPEONES_H
#define LATAPEONES_H

#include "grafo-escena.h"

class PeonNegro : public NodoGrafoEscena{
    public:
        PeonNegro();
};

class PeonBlanco : public NodoGrafoEscena{
    public:
        PeonBlanco();
};

class PeonMadera : public NodoGrafoEscena{
    public:
        PeonMadera();
};

class Inf_lata : public NodoGrafoEscena{
    public:
        Inf_lata();
};

class Sup_lata : public NodoGrafoEscena{
    public:
        Sup_lata();
};

class Centro_lata : public NodoGrafoEscena{
    public:
        Centro_lata();
};

class Lata : public NodoGrafoEscena{
    public: 
        Lata();
};

class LataPeones : public NodoGrafoEscena{
    public:
        LataPeones();
};

#endif