
cmake_minimum_required(VERSION 3.15)


project(online_store)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(PostgreSQL REQUIRED)
find_package(libpqxx REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCE_FILES
    src/main.cpp
    src/DatabaseConnection.cpp
    src/Product.cpp
    src/OrderItem.cpp
    src/PaymentStrategy.cpp
    src/Payment.cpp
    src/Order.cpp
    src/User.cpp
    src/Admin.cpp
    src/Manager.cpp
    src/Customer.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    libpqxx::pqxx
    ${PostgreSQL_LIBRARIES}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
endif()


message(STATUS "=========================================")
message(STATUS "Проект: Интернет-магазин")
message(STATUS "Студент: Храмцов Артём")
message(STATUS "Группа: ИУ1-12Б")
message(STATUS "=========================================")