#include "../include/Caserma.hpp"
#include <iostream>
#include <algorithm>//serve per find_if
using namespace std;

void Caserma::aggiungiPersonale(int id, const string& nome, Grado grado) {
    personale.push_back(make_shared<Personale>(id, nome, grado));
}

void Caserma::aggiungiMezzo(int id, const string& tipo) {
    // Qui dovremmo decidere se è aereo o terrestre. 
    // Per semplicità, se il nome contiene "Elicottero" o "Aereo" è aereo.
    if (tipo.find("Elicottero") != string::npos || tipo.find("Aereo") != string::npos) {
        mezzi.push_back(make_shared<MezzoAereo>(id, tipo));
    } else {
        mezzi.push_back(make_shared<MezzoTerrestre>(id, tipo));
    }
}

void Caserma::creaMissione(int tipoMissione, const string& descrizione,
                           const vector<int>& idPersonale,
                           const vector<int>& idMezzi) {
    
    int id = missioni.size() + 1;
    unique_ptr<Missione> nuovaMissione;

    switch(tipoMissione) {
        case 1: nuovaMissione = make_unique<MissioneScorta>(id, descrizione); break;
        case 2: nuovaMissione = make_unique<MissioneAssalto>(id, descrizione); break;
        case 3: nuovaMissione = make_unique<MissioneEstrazione>(id, descrizione); break;
        default: throw CasermaException("Tipo missione non valido");
    }

    // Assegnazione Personale
    for (int idP : idPersonale) {
        bool trovato = false;
        for (auto& p : personale) {
            if (p->getId() == idP) {
                if(p->isDisponibile()) {
                    nuovaMissione->assegnaPersonale(p);
                    trovato = true;
                } else {
                    cout << "Attenzione: Personale " << idP << " non disponibile.\n";
                }
            }
        }
        if (!trovato) cout << "Attenzione: Personale " << idP << " non trovato.\n";
    }

    // Assegnazione Mezzi
    for (int idM : idMezzi) {
        bool trovato = false;
        for (auto& m : mezzi) {
            if (m->getId() == idM) {
                if(m->isDisponibile()) {
                    nuovaMissione->assegnaMezzo(m);
                    trovato = true;
                } else {
                    cout << "Attenzione: Mezzo " << idM << " non disponibile.\n";
                }
            }
        }
        if (!trovato) cout << "Attenzione: Mezzo " << idM << " non trovato.\n";
    }

    // VALIDAZIONE FINALE
    try {
        nuovaMissione->valida();
        missioni.push_back(std::move(nuovaMissione)); // Sposto il pointer nel vettore
        cout << ">>> Missione creata e validata con successo.\n";
    } catch (const CasermaException& e) {
        // ROLLBACK: Se qualcosa va storto, dobbiamo "liberare" chi avevamo appena occupato!
        cout << "ERRORE VALIDAZIONE: " << e.what() << "\n";
        cout << ">>> Eseguo Rollback: ripristino disponibilità risorse...\n";

        // 1. Libero il personale
        for (int id : idPersonale) {
            for (auto& p : personale) { // 'personale' è il vector membro della classe
                if (p->getId() == id) {
                    p->setDisponibile(true);
                    break; // Trovato, usciamo dal ciclo interno
                }
            }
        }

    // 2. Libero i mezzi
        for (int id : idMezzi) {
            for (auto& m : mezzi) { // 'mezzi' è il vector membro della classe
                if (m->getId() == id) {
                    m->setDisponibile(true);
                    break; // Trovato
                }
            }
        }

        throw; // Rilancio l'errore al main affinché stampi il messaggio di avviso all'utente
    }
}

void Caserma::mostraPersonale() const {
    cout << "\n--- Personale ---\n";
    for (const auto& p : personale) {
        cout << p->getId() << " - " << p->getNome() << " (" << p->gradoToString()
             << ") [" << (p->isDisponibile() ? "Disponibile" : "In missione") << "]\n";
    }
}

void Caserma::mostraMezzi() const {
    cout << "\n--- Mezzi ---\n";
    for (const auto& m : mezzi) {
        cout << m->getId() << " - " << m->getTipo()
             << " [" << (m->isDisponibile() ? "Disponibile" : "In missione") << "]\n";
    }
}

void Caserma::mostraMissioni() const {
    cout << "\n=== Elenco Missioni ===\n";
    if(missioni.empty()) {
        cout << "Nessuna missione attiva.\n";
        return;
    }
    
    for (const auto& m : missioni) {
        m->mostraDettagli(); 
    }
}

void Caserma::terminaMissione(int idMissione) {
    auto it = missioni.begin();
    bool trovata = false;

    while (it != missioni.end()) {
        // (*it) è uno unique_ptr, quindi usiamo -> per chiamare i metodi di Missione
        if ((*it)->getId() == idMissione) {

            cout << "\n--------------------------------------------------\n";
            cout << " RIENTRO MISSIONE " << idMissione << " IN CORSO...\n";
            cout << "--------------------------------------------------\n";
            
            // 1. Liberiamo i soldati e i mezzi (tornano disponibili)
            (*it)->liberaRisorse();

            // 2. Rimuoviamo la missione dal vettore
            // erase restituisce l'iteratore all'elemento successivo, mantenendo valido il loop
            it = missioni.erase(it); 
            
            // Essendo unique_ptr, la rimozione dal vettore chiama automaticamente il distruttore della Missione, pulendo la memoria della missione stessa.
            
            cout << "Missione " << idMissione << " terminata e archiviata con successo.\n";
            cout << ">>> Controllare 'Officina' o 'Infermeria' per eventuali danni.\n";
            trovata = true;
            return; // Usciamo dalla funzione, lavoro finito
        } else {
            ++it; // Passiamo alla prossima missione
        }
    }

    if (!trovata) {
        cout << "Errore: Nessuna missione trovata con ID " << idMissione << ".\n";
    }
}

void Caserma::gestisciOfficina() {
    cout << "\n=== OFFICINA MECCANICA ===\n";
    bool trovati = false;
    for (auto& m : mezzi) {
        if (m->isGuasto()) {
            cout << "ID: " << m->getId() << " - " << m->getTipo() << " [DANNEGGIATO]\n";
            trovati = true;
        }
    }

    if (!trovati) {
        cout << "Nessun mezzo necessita di riparazioni.\n";
        return;
    }

    cout << "Inserisci ID del mezzo da riparare (0 per uscire): ";
    int id;
    cin >> id;
    if (id == 0) return;

    auto it = find_if(mezzi.begin(), mezzi.end(), [id](auto& m){ return m->getId() == id; });
    if (it != mezzi.end() && (*it)->isGuasto()) {
        (*it)->setGuasto(false);
        cout << "Mezzo riparato! Ora è operativo.\n";
    } else {
        cout << "Mezzo non trovato o non guasto.\n";
    }
}

void Caserma::gestisciInfermeria() {
    cout << "\n=== INFERMERIA ===\n";
    bool trovati = false;
    for (auto& p : personale) {
        if (p->isFerito()) {
            cout << "ID: " << p->getId() << " - " << p->getNome() << " [FERITO]\n";
            trovati = true;
        }
    }

    if (!trovati) {
        cout << "Nessun militare ferito.\n";
        return;
    }

    cout << "Inserisci ID del militare da curare (0 per uscire): ";
    int id;
    cin >> id;
    if (id == 0) return;

    auto it = find_if(personale.begin(), personale.end(), [id](auto& p){ return p->getId() == id; });
    if (it != personale.end() && (*it)->isFerito()) {
        (*it)->setFerito(false);
        cout << "Militare curato! Ora è abile all'arruolamento.\n";
    } else {
        cout << "Personale non trovato o sano.\n";
    }
}