#include "../snake/include/snake_model.h"

#include <gtest/gtest.h>

#include "../../constants/constants.h"
#include "../snake/include/state.h"
#include "mock_objects.h"

TEST(food_test, test_respawn) {
  s21::MockGenerator generator(22);
  std::vector<std::pair<int, int>> test_snake_body_coordinate;
  test_snake_body_coordinate.push_back({1, 3});

  s21::Food food(s21::kWidght, s21::kHight,
                 std::make_unique<s21::MockGenerator>(generator));

  food.Respawn(test_snake_body_coordinate);
  int x_checker = 1;
  int y_checker = 1;
  std::pair<int, int> check_coordinate = food.GetCoordinate();
  ASSERT_EQ(check_coordinate.first, x_checker);
  ASSERT_EQ(check_coordinate.second, y_checker);
}

TEST(food_test, test_accessors) {
  s21::MockGenerator generator(22);
  std::vector<std::pair<int, int>> test_snake_body_coordinate;
  test_snake_body_coordinate.push_back({1, 3});

  s21::Food food(s21::kWidght, s21::kHight,
                 std::make_unique<s21::MockGenerator>(generator));

  food.Respawn(test_snake_body_coordinate);
  int x_checker = 1;
  int y_checker = 1;
  std::pair<int, int> check_coordinate = food.GetCoordinate();
  ASSERT_EQ(check_coordinate.first, x_checker);
  ASSERT_EQ(check_coordinate.second, y_checker);

  ASSERT_EQ(food.x(), x_checker);
  ASSERT_EQ(food.y(), y_checker);
}

TEST(food_test, test_move_constructor) {
  s21::MockGenerator generator(22);
  std::vector<std::pair<int, int>> test_snake_body_coordinate;
  test_snake_body_coordinate.push_back({1, 3});

  s21::Food food(s21::kWidght, s21::kHight,
                 std::make_unique<s21::MockGenerator>(generator));

  food.Respawn(test_snake_body_coordinate);
  int x_checker = 1;
  int y_checker = 1;
  s21::Food food_2(std::move(food));

  bool checker = food.x() == food_2.x();

  ASSERT_FALSE(checker);
  checker = food.x() == food_2.x();
  ASSERT_FALSE(checker);

  ASSERT_EQ(food_2.x(), x_checker);
  ASSERT_EQ(food_2.y(), y_checker);
}

TEST(food_test, test_move_assigment) {
  s21::MockGenerator generator(22);
  std::vector<std::pair<int, int>> test_snake_body_coordinate;
  test_snake_body_coordinate.push_back({1, 3});

  s21::Food food(s21::kWidght, s21::kHight,
                 std::make_unique<s21::MockGenerator>(generator));

  food.Respawn(test_snake_body_coordinate);
  int x_checker = 1;
  int y_checker = 1;
  s21::Food food_2 = std::move(food);

  bool checker = food.x() == food_2.x();

  ASSERT_FALSE(checker);
  checker = food.x() == food_2.x();
  ASSERT_FALSE(checker);

  ASSERT_EQ(food_2.x(), x_checker);
  ASSERT_EQ(food_2.y(), y_checker);
}

TEST(model_test, test_start_state) {
  s21::SnakeModel model;

  model.SwitchingGameRun(true);

  model.Handle();

  ASSERT_EQ(model.get_game_status(), s21::GameStatus::kIdle);
  ASSERT_EQ(model.get_current_state(), &s21::SpawnState::GetInstance());
}

TEST(model_test, test_spawn_state) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  ASSERT_EQ(model.get_coordinate().size(), 4);
  ASSERT_EQ(model.get_level(), 1);
  ASSERT_EQ(model.get_game_status(), s21::GameStatus::kRun);
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
}

TEST(model_test, test_move_state) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
}

TEST(model_test, test_gameover_state) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется вперед пока не встретиться со стеной
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::GameOverState::GetInstance());
}

TEST(model_test, test_pause_state) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется вперед пока не встретиться со стеной
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());

  model.set_game_status(s21::GameStatus::kPause);

  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::PauseState::GetInstance());
}

TEST(model_test, test_pause_state_2) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется вперед пока не встретиться со стеной
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());

  model.set_game_status(s21::GameStatus::kPause);

  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::PauseState::GetInstance());

  model.set_game_status(s21::GameStatus::kRun);

  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
}

TEST(model_test, test_pause_state_3) {
  s21::SnakeModel model;

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется вперед пока не встретиться со стеной
  model.Handle();
  model.Handle();
  model.Handle();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());

  model.set_game_status(s21::GameStatus::kPause);

  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::PauseState::GetInstance());

  model.set_game_status(s21::GameStatus::kGameOver);
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::StartState::GetInstance());
}

TEST(model_test, test_eating_transision) {
  s21::MockGenerator m_gen(133);
  s21::SnakeModel model(std::make_unique<s21::MockGenerator>(m_gen));

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется
  model.Handle();
  model.TurnToDown();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::EatFoodState::GetInstance());
}

TEST(model_test, test_eating) {
  s21::MockGenerator m_gen(133);
  s21::SnakeModel model(std::make_unique<s21::MockGenerator>(m_gen));

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется
  model.Handle();
  model.TurnToDown();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::EatFoodState::GetInstance());
  model.Handle();
  ASSERT_EQ(model.get_score(), 1);
  ASSERT_EQ(model.get_high_score(), 1);
  ASSERT_EQ(model.get_lenght(), 5);
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
}

TEST(model_test, test_death_from_body) {
  s21::MockGenerator m_gen(133);
  s21::SnakeModel model(std::make_unique<s21::MockGenerator>(m_gen));

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется
  model.Handle();
  model.TurnToDown();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::EatFoodState::GetInstance());
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
  model.TurnToLeft();
  model.Handle();
  model.TurnToUp();
  model.Handle();
  model.TurnToRight();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::GameOverState::GetInstance());
}

TEST(model_test, test_back_to_move) {
  s21::MockGenerator m_gen(133);
  s21::SnakeModel model(std::make_unique<s21::MockGenerator>(m_gen));

  model.set_new_state(s21::SpawnState::GetInstance());

  model.Handle();

  // змейка движется
  model.Handle();
  model.TurnToDown();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::EatFoodState::GetInstance());
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
  model.TurnToLeft();
  model.Handle();
  model.TurnToUp();
  model.Handle();
  model.TurnToRight();
  model.Handle();

  ASSERT_EQ(model.get_current_state(), &s21::GameOverState::GetInstance());

  model.set_gameover_locker(false);
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::StartState::GetInstance());
  model.SwitchingGameRun(true);
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::SpawnState::GetInstance());
  model.Handle();
  ASSERT_EQ(model.get_current_state(), &s21::MoveState::GetInstance());
}

int main() {
  testing::InitGoogleTest();
  if (RUN_ALL_TESTS()) {
    std::cout << "Some tests have failed :(\n"
              << "Get back to work!" << std::endl;
  }
}