#include "../include/food.h"

s21::Food::Food(const int &max_x, const int &max_y,
                std::unique_ptr<IGenerator> generator)
    : coordinate_(), possible_positions_(), generator_(std::move(generator)) {
  for (int x = 0; x <= max_x; ++x) {
    for (int y = 0; y <= max_y; ++y) {
      possible_positions_.emplace(x, y);
    }
  }
}

s21::Food::Food(Food &&food_object) noexcept
    : coordinate_(std::move(food_object.coordinate_)),
      possible_positions_(std::move(food_object.possible_positions_)),
      generator_(std::move(food_object.generator_)) {
  food_object.coordinate_.first = 0;
  food_object.coordinate_.second = 0;
  food_object.possible_positions_.clear();
  food_object.generator_.release();
}

s21::Food::reference s21::Food::operator=(Food &&food_object) noexcept {
  if (this != &food_object) {
    coordinate_ = std::move(food_object.coordinate_);
    possible_positions_ = std::move(food_object.possible_positions_);
    generator_ = std::move(food_object.generator_);

    food_object.coordinate_.first = 0;
    food_object.coordinate_.second = 0;
    food_object.possible_positions_.clear();
    food_object.generator_.release();
  }

  return *this;
}

void s21::Food::Respawn(
    const std::vector<std::pair<int, int>> &snake_body_coordinate) {
  std::vector<std::pair<int, int>> removed_segments;

  for (const auto &segment : snake_body_coordinate) {
    auto it = possible_positions_.find(segment);

    if (it != possible_positions_.end()) {
      removed_segments.push_back(segment);
      possible_positions_.erase(it);
    }
  }

  if (!possible_positions_.empty()) {
    int distance =
        generator_.get()->GeneratePosition(possible_positions_.size() - 1);

    auto it = possible_positions_.begin();
    std::advance(it, distance);

    coordinate_.first = it->first;
    coordinate_.second = it->second;
  }

  for (const auto &segment : removed_segments) {
    possible_positions_.insert(segment);
  }
}

int s21::Food::x() const noexcept { return coordinate_.first; }

int s21::Food::y() const noexcept { return coordinate_.second; }

const std::pair<int, int> &s21::Food::GetCoordinate() const noexcept {
  return coordinate_;
}