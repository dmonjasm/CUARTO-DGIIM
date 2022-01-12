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

    TexturaXY * madera=new TexturaXY("../recursos/imgs/text-madera.jpg");

    agregar(new Material(madera,0.2,0.8,0.8,40));
    agregar(new MallaRevolPLY("../recursos/plys/peon.ply",100));
}

Peones::Peones(){
    ponerNombre("Peones");

    agregar(MAT_Traslacion(0.0,1.4,4.0));
    agregar(new PeonMadera());
    agregar(MAT_Traslacion(2.5,0.0,0.0));
    agregar(new PeonBlanco());
    agregar(MAT_Traslacion(2.5,0.0,0.0));
    agregar(new PeonNegro());
}

Sup_lata::Sup_lata(){
    ponerNombre("Parte Superior de la Lata");

    agregar(new Material(0.2,0.6,0.2,15));
    agregar(new MallaRevolPLY("../recursos/plys/lata_psup.ply",100));
}

Inf_lata::Inf_lata(){
    ponerNombre("Parte Inferior de la Lata");

    agregar(new Material(0.2,0.8,0.2,15));
    agregar(new MallaRevolPLY("../recursos/plys/lata_pinf.ply",100));
}

Centro_lata::Centro_lata(const std::string & path_imagen){
    ponerNombre("Cuerpo de la lata");

    Textura * logo=new Textura(path_imagen);

    agregar(new Material(logo,0.5,0.8,0.8,20));
    agregar(new MallaRevolPLY("../recursos/plys/lata_pcue.ply",100));
}

Lata::Lata(const std::string & path_imagen){
    ponerNombre("Lata completa");

    agregar(MAT_Escalado(3.0,3.0,3.0));
    agregar(new Centro_lata(path_imagen));
    agregar(new Inf_lata());
    agregar(new Sup_lata());
}


Lata_coke::Lata_coke(){
    ponerNombre("Lata coca-cola");
    ponerIdentificador(4);

    agregar(new Lata("../recursos/imgs/lata-coke.jpg"));
}

Lata_pepsi::Lata_pepsi(){
    ponerNombre("Lata pepsi");
    ponerIdentificador(5);

    agregar(new Lata("../recursos/imgs/lata-pepsi.jpg"));
}

Lata_ugr::Lata_ugr(){
    ponerNombre("Lata ugr");
    ponerIdentificador(6);

    agregar(new Lata("../recursos/imgs/window-icon.jpg"));
}

Latas::Latas(){
    ponerNombre("Latas con todas las imágenes");

    agregar(new Lata_coke());
    agregar(MAT_Traslacion(2.0,0.0,0.0));
    agregar(new Lata_pepsi());
    agregar(MAT_Traslacion(2.0,0.0,0.0));
    agregar(new Lata_ugr());
}

LataPeones::LataPeones(){
    ponerNombre("LataPeones");

    agregar(new Lata("../recursos/imgs/lata-coke.jpg"));
    agregar(new Peones());
}

VariasLatasPeones::VariasLatasPeones(){
    ponerNombre("LataPeones practica 5");

    agregar(new Latas());
    agregar(new Peones());
}