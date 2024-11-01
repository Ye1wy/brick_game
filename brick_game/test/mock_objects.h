#ifndef CPP_BRICKGAME_TEST_MOCK_OBJECTS_H
#define CPP_BRICKGAME_TEST_MOCK_OBJECTS_H

#include "../snake/include/interface_generator.h"

namespace s21 {
class MockGenerator : public IGenerator {
 public:
  MockGenerator(int num) : num_(num) {};
  ~MockGenerator() = default;

  int GeneratePosition(int generation_boundary) override {
    if (generation_boundary) {
      return num_;
    }

    return 0;
  };

 private:
  int num_;
};
}  // namespace s21

#endif