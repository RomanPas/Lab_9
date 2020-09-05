cmake_minimum_required(VERSION 3.2)

if($ENV{TRAVIS_BUILD_NUMBER})
	project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
	project(bayan VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS program_options filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(bayan main.cpp "lib/options.cpp" "lib/bayan_lib.cpp")

set_target_properties(bayan PROPERTIES
	CXX_STANDART 17
	CXX_STANDART_REQUIRED ON
)

target_link_libraries(bayan LINK_PUBLIC ${Boost_LIBRARIES})

if(MSVC)
	target_compile_options(bayan PRIVATE
		/W4
	)
else()
	target_compile_options(bayan PRIVATE
		-Wall -Wextra -pedantic -Werror
	)
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()