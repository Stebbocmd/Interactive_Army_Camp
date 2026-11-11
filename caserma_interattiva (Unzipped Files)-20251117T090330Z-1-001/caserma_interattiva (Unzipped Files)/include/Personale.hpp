#ifndef PERSONALE_HPP
#define PERSONALE_HPP
#include "Exceptions.hpp"
#include <string>
#include "TipiComuni.hpp"

/*enum class Grado {
    SOLDATO,
    CAPORALE,
    SERGENTE,
    TENENTE,
    CAPITANO,
    MAGGIORE
};*/ // Ora incluso da TipiComuni.hpp per non dover avere per forza un personale per ogni file che usa Grado

class Personale {
private:
    int id;
    std::string nome;
    Grado grado;
    bool disponibile;
    bool ferito = false; // NUOVO CAMPO (inizializzato a false)

public:
    Personale(int id, const std::string& nome, Grado grado);

    // Implementazione inline per semplicità
    int getId() const;
    std::string getNome() const;
    Grado getGrado() const;
    bool isDisponibile() const;
    void setDisponibile(bool disp);

    bool isFerito() const { return ferito; }      // Getter
    void setFerito(bool stato) { ferito = stato; } // Setter

    std::string gradoToString() const {
        switch (grado) {
            case Grado::SOLDATO: return "Soldato";
            case Grado::CAPORALE: return "Caporale";
            case Grado::SERGENTE: return "Sergente";
            case Grado::TENENTE: return "Tenente";
            case Grado::CAPITANO: return "Capitano";
            case Grado::MAGGIORE: return "Maggiore";
            default: return "Sconosciuto";
        }
    }
};

#endif
