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

class Peones : public NodoGrafoEscena{
    public:
        Peones();
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
        Centro_lata(const std::string & path_imagen);
};

class Lata : public NodoGrafoEscena{
    public: 
        Lata(const std::string & path_imagen);
};

class Lata_coke : public NodoGrafoEscena{
    public:
        Lata_coke();
};

class Lata_pepsi : public NodoGrafoEscena{
    public:
        Lata_pepsi();
};

class Lata_ugr : public NodoGrafoEscena{
    public:
        Lata_ugr();
};

class Latas : public NodoGrafoEscena{
    public:
        Latas();
};


class LataPeones : public NodoGrafoEscena{
    public:
        LataPeones();
};

class LataPeones_def : public NodoGrafoEscena{
    public:
        LataPeones_def();
};


#endif