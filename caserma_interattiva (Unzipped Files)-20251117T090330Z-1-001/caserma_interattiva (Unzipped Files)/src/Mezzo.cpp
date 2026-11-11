#include "../include/Mezzo.hpp"
#include <iostream>
using namespace std;

Mezzo::Mezzo(int id, const string& tipo) : id(id), tipo(tipo), disponibile(true) {}

//getter e setter
int Mezzo::getId() const { return id; }
bool Mezzo::isDisponibile() const { return disponibile && !guasto; }//disponibile solo se non è guasto e non in missione
void Mezzo::setDisponibile(bool disp) { disponibile = disp; }

bool Mezzo::isGuasto() const { return guasto; }
void Mezzo::setGuasto(bool stato) { guasto = stato; }
    
// REQUISITO 1: Controllo equipaggio (Max 6)
void Mezzo::aggiungiMembroEquipaggio(shared_ptr<Personale> p) {
    // Controlliamo solo il limite massimo qui.
    // Il limite minimo si controlla quando parte la missione.
    if (equipaggio.size() >= 6) {
        throw CasermaException("Impossibile aggiungere membro al mezzo " + to_string(id) + 
                               ": Equipaggio al completo (max 6).");
    }

    // Controllo disponibilità soldato
    if (!p->isDisponibile()) {
        throw CasermaException("Errore: Il personale " + p->getNome() + " non è disponibile.");
    }

    // Assegna il personale al mezzo
    equipaggio.push_back(p);
    p->setDisponibile(false); // Il personale ora è occupato
}

    // Validazione pre-partenza (Req 1: Min 2)
void Mezzo::checkEquipaggioCompleto() const {
    if (equipaggio.size() < 2) {
        throw CasermaException("Il mezzo " + tipo + " (ID: " + std::to_string(id) + 
                               ") non può partire: equipaggio insufficiente (" + 
                               std::to_string(equipaggio.size()) + "/2 min).");
    }
}
