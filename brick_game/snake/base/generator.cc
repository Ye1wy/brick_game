#include "../include/generator.h"

s21::Generator::Generator()
    : seed_(std::chrono::system_clock::now().time_since_epoch().count()),
      generator_(std::make_unique<std::mt19937_64>(seed_)) {}

s21::Generator::Generator(Generator&& generator_object) noexcept
    : seed_(std::move(generator_object.seed_)),
      generator_(std::move(generator_object.generator_)) {
  generator_object.seed_ = 0;
  generator_object.generator_.release();
}

s21::Generator::reference s21::Generator::operator=(
    Generator&& generator_object) noexcept {
  if (this != &generator_object) {
    seed_ = std::move(generator_object.seed_);
    generator_ = std::move(generator_object.generator_);

    generator_object.seed_ = 0;
    generator_object.generator_.release();
  }

  return *this;
}

int s21::Generator::GeneratePosition(int limit) {
  uniform_dist dist(0, limit);
  int res = dist(*generator_);
  return res;
}