# Makefile для структуры math_labs с общими функциями и отдельными лабораторными работами
# Поддерживает автоматическую сборку библиотеки и всех лабораторных работ

# Настройки компилятора
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fopenmp
DEBUG_FLAGS = -g -DDEBUG
INCLUDES = -Iinclude

# Директории
SRC_DIR = src
INCLUDE_DIR = include
LABS_DIR = labs
OBJ_DIR = obj

TEMP_DIR = temp

# Библиотека
LIB_NAME = libfunctions.a

# Поиск исходников библиотеки
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Поиск лабораторных работ (автоматическое обнаружение)
LABS = $(patsubst $(LABS_DIR)/%/main.c, %, $(wildcard $(LABS_DIR)/*/main.c))

# Цели по умолчанию
.PHONY: all clean help library labs list debug init
.DEFAULT_GOAL := help

# Справка
help:
	@echo "Makefile для проекта math_labs"
	@echo "=============================="
	@echo ""
	@echo "Доступные команды:"
	@echo "  all          - Собрать библиотеку и все лабораторные работы"
	@echo "  library      - Собрать только библиотеку libmathfuncs.a"
	@echo "  labs         - Собрать все лабораторные работы"
	@echo "  list         - Показать список доступных лабораторных работ"
	@echo "  lab1, lab2   - Собрать конкретную лабораторную работу"
	@echo "  debug        - Собрать с отладочной информацией"
	@echo "  clean        - Удалить все скомпилированные файлы"
	@echo "  init         - Создать необходимые директории"
	@echo ""
	@echo "Примеры использования:"
	@echo "  make library     # Собрать библиотеку"
	@echo "  make lab1        # Собрать первую лабораторную"
	@echo "  make all         # Собрать всё"
	@echo "  make clean       # Очистить проект"

# Показать список лабораторных работ
list:
	@echo "Найденные лабораторные работы:"
	@if [ -z "$(LABS)" ]; then \
		echo "  (лабораторные работы не найдены)"; \
	else \
		for lab in $(LABS); do \
			echo "  $$lab"; \
		done; \
	fi

# Собрать всё
all: clean-temp library labs

# Собрать библиотеку
library: $(LIB_NAME)

# Собрать все лабораторные работы
labs: library $(LABS)

# Правило сборки библиотеки
$(LIB_NAME): $(OBJECTS)
	@echo "Создание библиотеки $(LIB_NAME)..."
	ar rcs $@ $^
	@echo "✅ Библиотека $(LIB_NAME) успешно создана"

# Компиляция объектных файлов библиотеки
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/functions.h
	@mkdir -p $(OBJ_DIR)
	@echo "Компиляция $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Правила для сборки лабораторных работ
define LAB_RULE
$(1): $(LABS_DIR)/$(1)/main.c $(LIB_NAME)
	@echo "Сборка лабораторной работы: $(1)"
	$(CC) $(CFLAGS) $(INCLUDES) $$< -L. -lfunctions -lm -o $$@
	@echo "✅ Лабораторная работа $(1) собрана"

.PHONY: $(1)
endef

# Генерация правил для всех найденных лабораторных работ
$(foreach lab,$(LABS),$(eval $(call LAB_RULE,$(lab))))

# Сборка с отладочной информацией
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

# Создание необходимых директорий
init:
	@echo "Создание структуры проекта..."
	@mkdir -p $(SRC_DIR) $(INCLUDE_DIR) $(LABS_DIR) $(OBJ_DIR)
	@mkdir -p $(LABS_DIR)/lab1 $(LABS_DIR)/lab2 $(LABS_DIR)/lab3
	@echo "✅ Структура проекта создана"

# Правила для запуска лабораторных работ
define RUN_RULE
run-$(1): $(1)
	@echo "Запуск лабораторной работы: $(1)"
	@./$(1)

.PHONY: run-$(1)
endef

# Генерация правил запуска для всех лабораторных работ
$(foreach lab,$(LABS),$(eval $(call RUN_RULE,$(lab))))

# Информация о проекте
info:
	@echo "Информация о проекте math_labs:"
	@echo "  Компилятор: $(CC)"
	@echo "  Флаги: $(CFLAGS)"
	@echo "  Библиотека: $(LIB_NAME)"
	@echo "  Исходники: $(words $(SOURCES)) файлов"
	@echo "  Лабораторные: $(words $(LABS)) работ"
	@if [ -n "$(LABS)" ]; then \
		echo "  Доступные лабораторные работы:"; \
		for lab in $(LABS); do \
			echo "    - $$lab"; \
		done; \
	fi

# Проверка зависимостей
check:
	@echo "Проверка структуры проекта..."
	@test -d $(SRC_DIR) || (echo "❌ Директория $(SRC_DIR) не найдена" && exit 1)
	@test -d $(INCLUDE_DIR) || (echo "❌ Директория $(INCLUDE_DIR) не найдена" && exit 1)
	@test -d $(LABS_DIR) || (echo "❌ Директория $(LABS_DIR) не найдена" && exit 1)
	@test -f $(INCLUDE_DIR)/functions.h || (echo "❌ Файл $(INCLUDE_DIR)/functions.h не найден" && exit 1)
	@echo "✅ Структура проекта корректна"

# Очистка содержимого temp, но не удаление папки
clean-temp:
	@rm -f $(TEMP_DIR)/*

# Очистка
clean:
	@echo "Очистка проекта..."
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_NAME)
	rm -f $(LABS)
	@echo "✅ Проект очищен"

# Полная пересборка
rebuild: clean all

# Установка библиотеки в систему (опционально)
install: library
	@echo "Установка библиотеки в систему..."
	sudo cp $(LIB_NAME) /usr/local/lib/
	sudo mkdir -p /usr/local/include/mathfuncs
	sudo cp $(INCLUDE_DIR)/*.h /usr/local/include/mathfuncs/
	sudo ldconfig
	@echo "✅ Библиотека установлена"