#ifndef CPP_BRICKGAME_INCLUDE_FOOD_H
#define CPP_BRICKGAME_INCLUDE_FOOD_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <set>
#include <vector>

#include "generator.h"
#include "interface_generator.h"

namespace s21 {
/// @brief A class that generates new food coordinates for the model object
class Food {
 public:
  using reference = Food&;
  /// @brief  The default constructor with parameters that indicate in which
  /// frames the coordinates of the food appear and with which generator
  /// @param max_x
  /// @param max_y
  /// @param generator  there is no parameter, then the native generator is set
  /// by default
  Food(const int& max_x, const int& max_y,
       std::unique_ptr<IGenerator> generator = std::make_unique<Generator>());
  explicit Food(const Food&) = delete;
  Food(Food&&) noexcept;
  ~Food() = default;

  reference operator=(const Food&) = delete;
  reference operator=(Food&&) noexcept;

  /// @brief This method finds a place where you can generate new food by
  /// removing the coordinates of the snake from the range
  /// @param snake_body_coordinate the current coordinates of the snake
  void Respawn(const std::vector<std::pair<int, int>>& snake_body_coordinate);
  /// @brief Accessor for food x coordinate
  /// @return food x coordinate. type int
  int x() const noexcept;
  /// @brief Accessor for food y coordinate
  /// @return food y coordinate. type int
  int y() const noexcept;
  /// @brief Accessor for food pair<x,y> coordinate
  /// @return food x,y coordinate, type pair<int, int>
  const std::pair<int, int>& GetCoordinate() const noexcept;

 private:
  std::pair<int, int> coordinate_;
  std::set<std::pair<int, int>> possible_positions_;
  std::unique_ptr<IGenerator> generator_;
};

}  // namespace s21

#endif