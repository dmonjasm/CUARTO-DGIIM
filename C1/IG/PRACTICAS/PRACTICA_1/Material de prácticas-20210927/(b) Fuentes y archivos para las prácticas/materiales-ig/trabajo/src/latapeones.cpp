// Nombre: Daniel, Apellidos: Monjas Miguélez, DNI/Pasaporte: 70274432W (IG GIM+GIADE, curso 21-22)

#include "latapeones.h"
#include "malla-revol.h"

PeonBlanco::PeonBlanco(){
    ponerNombre("Peon Blanco");
    ponerIdentificador(1);

    ponerColor({1.0,1.0,1.0});

    agregar(new Material(0.2,0.8,0.2,15));
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply",100));
}

PeonNegro::PeonNegro(){
    ponerNombre("Peon Negro");
    ponerIdentificador(2);

    ponerColor({0.0,0.0,0.0});

    agregar(new Material(0.2,0.2,0.8,15));
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply",100));
}

PeonMadera::PeonMadera(){
    ponerNombre("Peon Madera");
    ponerIdentificador(3);

    Textura * madera=new Textura("../recursos/imgs/text-madera.jpg");

    agregar(new Material(madera,0.2,0.8,0.8,40));
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply",100));
}

Sup_lata::Sup_lata(){
    ponerNombre("Parte Superior de la Lata");
    ponerIdentificador(4);

    agregar(new Material(0.2,0.6,0.2,15));
    agregar(new MallaRevolPLY("../recursos/plys/lata_psup.ply",100));
}

Inf_lata::Inf_lata(){
    ponerNombre("Parte Inferior de la Lata");
    ponerIdentificador(5);

    agregar(new Material(0.2,0.8,0.2,15));
    agregar(new MallaRevolPLY("../recursos/plys/lata_pinf.ply",100));
}

Centro_lata::Centro_lata(){
    ponerNombre("Cuerpo de la lata");
    ponerIdentificador(6);

    Textura * logo=new Textura("../recursos/imgs/lata-coke.jpg");

    agregar(new Material(logo,0.5,0.8,0.8,20));
    agregar(new MallaRevolPLY("../recursos/plys/lata_pcue.ply",100));
}

Lata::Lata(){
    ponerNombre("Lata completa");
    ponerIdentificador(7);

    agregar(new Centro_lata());
    agregar(new Inf_lata());
    agregar(new Sup_lata());
}

LataPeones::LataPeones(){
    ponerNombre("LataPeones");
    ponerIdentificador(8);

    agregar(new Lata());
    agregar(MAT_Escalado(0.25,0.25,0.25));
    agregar(MAT_Traslacion(0.0,1.4,2.2));
    agregar(new PeonMadera());
    agregar(MAT_Traslacion(2.2,0.0,0.0));
    agregar(new PeonBlanco());
    agregar(MAT_Traslacion(2.2,0.0,0.0));
    agregar(new PeonNegro());
}