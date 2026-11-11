#ifndef CASERMA_HPP
#define CASERMA_HPP

#include "Personale.hpp"
#include "Mezzo.hpp"
#include "Missione.hpp"
#include <vector>
#include <memory>
#include <string>


class Caserma {
    private:
        std::vector<std::shared_ptr<Personale>> personale;
        std::vector<std::shared_ptr<Mezzo>> mezzi;
        std::vector<std::unique_ptr<Missione>> missioni;  //Qui basta un unique_ptr poichè la missione è "condivisa" e usata solamente all'interno della caserma e non in altre aree
   public:
    // Accettiamo parametri per valore o reference, ma internamente creiamo smart pointers
    void aggiungiPersonale(int id, const std::string& nome, Grado grado);
    void aggiungiMezzo(int id, const std::string& tipo);
    
    void creaMissione(int tipoMissione, const std::string& descrizione,
                      const std::vector<int>& idPersonale,
                      const std::vector<int>& idMezzi);    
    
    void terminaMissione(int idMissione);
    void mostraPersonale() const;
    void mostraMezzi() const;
    void mostraMissioni() const;
    void gestisciOfficina();   // Mostra mezzi guasti e ripara
    void gestisciInfermeria(); // Mostra feriti e cura

    
};

#endif
