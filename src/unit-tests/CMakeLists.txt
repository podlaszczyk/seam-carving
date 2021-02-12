find_package(Catch2 REQUIRED)

add_executable(UnitTests unit-tests.cpp tests-main.cpp)
target_link_libraries(UnitTests Catch2::Catch2)
target_link_libraries(UnitTests sc)

include(ParseAndAddCatchTests)
ParseAndAddCatchTests(UnitTests)
