#include "../include/Missione.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Missione::Missione(int id, const string& descrizione) : id(id), descrizione(descrizione) {}

void Missione::assegnaPersonale(shared_ptr<Personale> p) {
    personaleAssegnato.push_back(p);
    p->setDisponibile(false);
}

void Missione::assegnaMezzo(shared_ptr<Mezzo> m) {
    mezziAssegnati.push_back(m);
    m->setDisponibile(false);
}


void Missione::mostraDettagli() const {
    cout << "=== IdMissione == " << id << " ===\n";
    cout << "Descrizione: " << descrizione << "\n";
    cout << "Missione di tipo: " << getTipoMissione() << endl; 

    cout << "Personale assegnato:\n";
    for (const auto& p : personaleAssegnato)
        cout << " - " << p->getNome() << " (" << p->gradoToString() << ")\n";

    cout << "Mezzi assegnati:\n";
    for (const auto& m : mezziAssegnati)
        cout << " - " << m->getTipo() << "\n";
}

int Missione::getId() const { 
    return id; 
}

// Implementazione di liberaRisorse
void Missione::liberaRisorse() {
    // Rende di nuovo disponibile tutto il personale
    cout << "Rapporto di missione:\n";
    for (auto& p : personaleAssegnato) {
        p->setDisponibile(true);
        //20% probabilità ferita
        if ((rand() % 100) < 20) { 
            p->setFerito(true);
            cout << " [!] MILITARE FERITO: " << p->getNome() << "\n";
        }
    }

    // Rende di nuovo disponibili tutti i mezzi
    for (auto& m : mezziAssegnati) {
        m->setDisponibile(true);
        // 30% probabilità guasto
        if ((rand() % 100) < 30) {
            m->setGuasto(true);
            cout << " [!] MEZZO DANNEGGIATO: " << m->getTipo() << " (ID: " << m->getId() << ")\n";
        }
    }

    cout << "Risorse della missione " << id << " liberate.\n";
    

}