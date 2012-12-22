#ifndef INPUT_GENERATOR_EXCEPTION_HPP_
#define INPUT_GENERATOR_EXCEPTION_HPP_

#include <exception>
#include <string>

namespace inputGenerator {

class Exception: public std::exception {
  public:
    Exception(const std::string &_message): message(_message) {}
    virtual const char* what() const throw() {
        return message.c_str();
    }

    virtual ~Exception() throw() {};
    const std::string message;
};

}

#endif // INPUT_GENERATOR_EXCEPTION_HPP_
