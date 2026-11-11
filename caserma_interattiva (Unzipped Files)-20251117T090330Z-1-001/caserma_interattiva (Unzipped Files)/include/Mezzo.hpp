#ifndef MEZZO_HPP
#define MEZZO_HPP
#include "Exceptions.hpp"
#include "Personale.hpp"
#include <string>
#include<iostream>
#include <vector>
#include <memory>

class Mezzo {// classe astratta 
private:
    int id;
    std::string tipo;
    bool disponibile;
    std::vector<std::shared_ptr<Personale>> equipaggio;
    bool guasto = false;//stato del veicolo

public:
    Mezzo(int id, const std::string& tipo);

    virtual ~Mezzo() = default;

    int getId () const;
    /*std::string getTipo() const;*/
    bool isDisponibile() const;
    void setDisponibile(bool disp);
    // Metodi virtuali puri (devono essere implementati dalle sottoclassi)
    virtual bool isTerrestre() const = 0;
    virtual std::string getTipo() const = 0;
    void aggiungiMembroEquipaggio(std::shared_ptr<Personale> p);
    void checkEquipaggioCompleto() const;
    void setGuasto(bool stato);
    bool isGuasto() const;
};

class MezzoTerrestre : public Mezzo {
    public:
        MezzoTerrestre(int id, const std::string& modello) : Mezzo(id, modello) {}
        bool isTerrestre() const override { return true; }
        std::string getTipo() const { return "Mezzo Terrestre"; }
};

class MezzoAereo : public Mezzo {
    public:
        MezzoAereo(int id, const std::string& modello) : Mezzo(id, modello) {}
        bool isTerrestre() const override { return false; }
        std::string getTipo() const { return "Mezzo Aereo"; }
};


#endif
