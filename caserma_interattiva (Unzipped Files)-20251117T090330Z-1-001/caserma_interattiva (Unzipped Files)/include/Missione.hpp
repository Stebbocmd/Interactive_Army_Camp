#ifndef MISSIONE_HPP   
#define MISSIONE_HPP   

#include "Personale.hpp"
#include "Mezzo.hpp"
#include <string>
#include <vector>
#include <memory> // Necessario per std::shared_ptr

class Missione {
protected:
    int id;
    std::string descrizione;
    std::vector<std::shared_ptr<Personale>> personaleAssegnato; // Personale extra (fanteria, specialisti)
    std::vector<std::shared_ptr<Mezzo>> mezziAssegnati;   // Mezzi assegnati alla missione

public:
    Missione(int id, const std::string& descrizione);
    virtual ~Missione() = default;

    // Metodi che accettano smart pointers
    void assegnaPersonale(std::shared_ptr<Personale> p);
    void assegnaMezzo(std::shared_ptr<Mezzo> m);

    void mostraDettagli() const;

    virtual void valida() const = 0;
    int getId() const;      // Getter semplice
    void liberaRisorse(); // Rende disponibile personale e mezzi
    virtual std::string getTipoMissione() const = 0;
};

// --- SOTTOCLASSI ---

// 1  SCORTA
class MissioneScorta : public Missione {
public:
    using Missione::Missione;
    std::string getTipoMissione() const override{return "Scorta";}
    void valida () const override{
        if (mezziAssegnati.size()<3){
            throw CasermaException("Missione Scorta: impossbile avviarla con meno di 3 veicoli");
        }
    }
};


//2  ASSALTO
class MissioneAssalto : public Missione {
public:
    using Missione::Missione;
    std::string getTipoMissione() const override{ return "Assalto"; }
    void valida() const override {
        // Il personale qui è la personaleAssegnati di fanteria/assalto, non l'equipaggio dei mezzi.
        if (mezziAssegnati.size() > 0 && personaleAssegnato.size() == 0) {
            throw CasermaException("Missione Assalto: impossibile avere mezzi senza fanteria.");
        }
       // se ho 5 persone -> max 1 mezzo. Se ho 4 persone -> 0 mezzi?
       // Formula: num_mezzi <= num_personale / 5
       size_t maxMezzi = personaleAssegnato.size() / 5;
       if (mezziAssegnati.size() > maxMezzi) {
           throw CasermaException("Missione Assalto: Troppi mezzi! (Max 1 ogni 5 soldati). Hai " + 
                                  std::to_string(mezziAssegnati.size()) + " soldati, max " + 
                                  std::to_string(maxMezzi) + " mezzi.");
        }
   }
};

// 3 ESTRAZIONE (almeno 2 mezzi terrestri e 5 elementi di personale)
class MissioneEstrazione : public Missione {
public:
    using Missione::Missione;
    std::string getTipoMissione() const override{ return "Estrazione"; }
    void valida() const override{ 
        if (personaleAssegnato.size() < 5) {
            throw CasermaException("Missione Estrazione richiede almeno 5 soldati.");
        }
        int terrestri = 0;
        for (const auto& m : mezziAssegnati) {
            if (m->isTerrestre()) terrestri++;
        }
        if (terrestri < 2) {
            throw CasermaException("Missione Estrazione richiede almeno 2 mezzi terrestri.");
        }
    
    }
};


#endif