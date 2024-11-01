#ifndef CPP_BRICKGAME_INCLUDE_INTERFACE_GENERATOR_H
#define CPP_BRICKGAME_INCLUDE_INTERFACE_GENERATOR_H

namespace s21 {
/// @brief An interface from which you can inherit to create a custom generator
class IGenerator {
 public:
  virtual ~IGenerator() = default;
  virtual int GeneratePosition(int) = 0;
};
}  // namespace s21

#endif