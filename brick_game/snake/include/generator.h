#ifndef CPP_BRICKGAME_INCLUDE_GENERATOR_H
#define CPP_BRICKGAME_INCLUDE_GENERATOR_H

#include <chrono>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>

#include "interface_generator.h"

namespace s21 {

using uniform_dist = std::uniform_int_distribution<int>;

/// @brief The generator class provides a random number from a given limit
class Generator : public IGenerator {
 public:
  using reference = Generator&;

  Generator();
  Generator(const Generator& generator_object) = delete;
  Generator(Generator&& generator_object) noexcept;
  ~Generator() = default;

  reference operator=(const Generator& generator_object) = delete;
  reference operator=(Generator&& generator_object) noexcept;
  /// @brief The method of generating a number within a certain range from 0 to
  /// limit
  /// @param limit the boundary for the generation number
  /// @return random number in range from 0 to limit
  int GeneratePosition(int limit) override;

 private:
  unsigned seed_;
  std::unique_ptr<std::mt19937_64> generator_;
};

}  // namespace s21

#endif