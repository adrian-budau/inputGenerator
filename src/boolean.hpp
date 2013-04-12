#ifndef INPUT_GENERATOR_BOOLEAN_HPP_
#define INPUT_GENERATOR_BOOLEAN_HPP_

namespace inputGenerator {

class Boolean {
  public:
    class Object {
      public:
        Object(const Object& that):
            data_(that.data_) {
        }

        explicit operator bool() const {
            return data_;
        }

        friend class Boolean;

      private:
        explicit Object(const bool &value = false):
            data_(value) {
        }

        bool data_;
    };

    static const Object True;
    static const Object False;
};

const Boolean::Object Boolean::True = Object(true);
const Boolean::Object Boolean::False  = Object(false);

}  // namespace inputGenerator

#endif  // INPUT_GENERATOR_BOOLEAN_HPP_
