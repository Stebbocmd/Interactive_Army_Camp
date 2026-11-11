#include "../include/Personale.hpp"
using namespace std;

Personale::Personale(int id, const string& nome, Grado grado) : id(id), nome(nome), grado(grado), disponibile(true) {}

int Personale::getId() const { return id; }
string Personale::getNome() const { return nome; }
Grado Personale::getGrado() const { return grado; }
bool Personale::isDisponibile() const { return disponibile; }
void Personale::setDisponibile(bool disp) { disponibile = disp; }


