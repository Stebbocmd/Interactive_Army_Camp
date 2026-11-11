#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

// Eccezione base per la logica della Caserma
class CasermaException : public std::runtime_error {
public:
    explicit CasermaException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif

