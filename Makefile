CXX=g++
OS=$(shell uname)
FLAGS=-Werror -Wall -Wextra -std=c++17 -D __cli
CURSESFLAG=-lncurses
TARGETS=cli_main.cc
OBJ_DIR=obj
LIB_DIR=lib
TEST_OBJ_DIR=$(OBJ_DIR)/test

# Параметры для тестирования
TESTS=brick_game/test/*.cc
ifeq ($(OS), Linux)
    TEST_FLAGS=-lcheck -lgmock -lgtest -lrt -lm
else
    TEST_FLAGS=-lcheck -lgmock -lgtest
endif


# Источники
SOURCES_SNAKE_BACKEND=$(wildcard brick_game/snake/base/*.cc)
SOURCES_TETRIS_BACKEND=$(wildcard brick_game/tetris/base/*.cc)
SOURCES_TETRIS_LEGACY=$(wildcard brick_game/tetris/base/legacy/*.c)
SOURCES_CLI=gui/cli/base/view.cc timer/console_time_provider.cc
SOURCES_CONTROLLER=controller/snake/base/snake_controller.cc \
                   controller/base/controller.cc \
                   controller/tetris/base/tetris_controller.cc

# Создание списка объектных файлов для каждой библиотеки
OBJS_SNAKE=$(patsubst %.cc, $(OBJ_DIR)/%.o, $(notdir $(SOURCES_SNAKE_BACKEND)))
OBJS_TETRIS=$(patsubst %.cc, $(OBJ_DIR)/%.o, $(notdir $(SOURCES_TETRIS_BACKEND)))
OBJS_TETRIS_LEGACY=$(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SOURCES_TETRIS_LEGACY)))
OBJS_CONTROLLER=$(patsubst %.cc, $(OBJ_DIR)/%.o, $(notdir $(SOURCES_CONTROLLER)))
OBJS_CLI=$(patsubst %.cc, $(OBJ_DIR)/%.o, $(notdir $(SOURCES_CLI)))
OBJS_MAIN=$(patsubst %.cc, $(OBJ_DIR)/%.o, $(notdir $(TARGETS)))
TEST_OBJS=$(patsubst %.cc, $(TEST_OBJ_DIR)/%.o, $(notdir $(TESTS)))

all: rebuild

dvi:
	doxygen doxygen/Doxyfile
ifeq ($(UNAME_S), Darwin)
	@open doxygen/html/index.html
else
	@xdg-open doxygen/html/index.html
endif

install: cli_v desktop_v

uninstall:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -rf data
	rm -rf desktop_build
	rm -f GameCli
	rm -f GameDesktop

dist: clean
	mkdir -p dist
	cp -R brick_game dist
	cp -R constants dist
	cp -R controller dist
	cp -R doxygen dist
	cp -R gui dist
	cp -R timer dist
	cp cli_main.cc dist
	cp FSM.png dist
	cp info.md dist
	cp Makefile dist
	tar cvzf brick_game_dist.tgz dist
	rm -rf dist

snake.a: $(OBJ_DIR) $(LIB_DIR) $(OBJS_SNAKE)
	ar rcs $(LIB_DIR)/libsnake.a $(OBJS_SNAKE)

tetris.a: $(OBJ_DIR) $(LIB_DIR) $(OBJS_TETRIS_LEGACY) $(OBJS_TETRIS)
	ar rcs $(LIB_DIR)/libtetris.a $(OBJS_TETRIS_LEGACY) $(OBJS_TETRIS)

$(OBJ_DIR)/%.o: brick_game/snake/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: brick_game/tetris/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: brick_game/tetris/base/legacy/%.c | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: controller/snake/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: controller/tetris/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: controller/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: gui/cli/base/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: timer/%.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.cc | $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(TEST_OBJ_DIR)/%.o: brick_game/test/%.cc | $(TEST_OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

cli_v: snake.a tetris.a $(OBJS_MAIN) $(OBJS_CLI) $(OBJS_CONTROLLER)
	$(CXX) $(OBJS_MAIN) $(OBJS_CLI) $(OBJS_CONTROLLER) -L$(LIB_DIR) -lsnake -ltetris $(FLAGS) $(CURSESFLAG) -o GameCli
	mkdir -p data
	touch data/score_table.txt

desktop_v:
	mkdir -p desktop_build
	cd desktop_build && qmake CONFIG+=debug ../gui/desktop/QtView
	cd desktop_build && make
	mv desktop_build/QtView . && mv QtView GameDesktop

tests: clean snake.a test_objects
	$(CXX) $(FLAGS) $(TEST_OBJS) $(OBJS_SNAKE) -L$(LIB_DIR) -lsnake -lgmock -lgtest -o test
	./test

test_objects: $(TEST_OBJ_DIR) $(TEST_OBJS)

gcov_report: clean
	lcov -z -d .
	$(CXX) $(FLAGS) --coverage $(TESTS) $(SOURCES_SNAKE_BACKEND) $(TEST_FLAGS) -o gcov_test
	./gcov_test
	lcov -t "snake_test" -o test.info --ignore-errors empty --ignore-errors inconsistent --no-external -c -d .
	genhtml -o report test.info --ignore-errors missing
ifeq ($(UNAME_S), Darwin)
	@open /report/index.html
else
	@xdg-open report/index.html
endif

rebuild: uninstall install

clean: uninstall
	rm -rf *.o *.gcno *.gcda *.info test gcov_report report greport a.out *.txt *.dSYM *.tgz gcov_test doxygen/html doxygen/latex
	
style:
	clang-format -n --style=google brick_game/tetris/base/*.cc brick_game/snake/base/*.cc brick_game/snake/include/*.h brick_game/tetris/include/*.h brick_game/tetris/base/legacy/*.c brick_game/tetris/include/legacy/*.h controller/base/*.cc controller/include/*.h controller/snake/base/*.cc controller/snake/include/*.h controller/tetris/base/*.cc controller/tetris/include/*.h constants/*.h timer/*.cc timer/*.h gui/cli/base/*.cc gui/cli/include/*.h gui/desktop/QtView/*.cc gui/desktop/QtView/*.h gui/include/*.h

style_correct:
	clang-format -i --style=google brick_game/tetris/base/*.cc brick_game/snake/base/*.cc brick_game/snake/include/*.h brick_game/tetris/include/*.h brick_game/tetris/base/legacy/*.c brick_game/tetris/include/legacy/*.h controller/base/*.cc controller/include/*.h controller/snake/base/*.cc controller/snake/include/*.h controller/tetris/base/*.cc controller/tetris/include/*.h constants/*.h timer/*.cc timer/*.h gui/cli/base/*.cc gui/cli/include/*.h gui/desktop/QtView/*.cc gui/desktop/QtView/*.h gui/include/*.h


