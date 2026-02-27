cmake_minimum_required(VERSION 3.15)
project(RestaurantSystem VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск libpqxx
find_package(libpqxx REQUIRED)

# Исполняемый файл
add_executable(restaurant_system
    src/lab4.cpp
    src/Database.cpp
    src/Logger.cpp
    src/Category.cpp
    src/Dish.cpp
    src/Order.cpp
)

# Подключение библиотек
target_link_libraries(restaurant_system PRIVATE libpqxx::pqxx)

# Папка для выходных файлов
set_target_properties(restaurant_system PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)