cmake_minimum_required(VERSION 3.12)
project(RobotArm_CCD)

set(CMAKE_CXX_STANDARD 14)

add_executable(RobotArm_CCD main.cpp CCD/Vector3.cpp CCD/Vector3.h CCD/Point.cpp CCD/Point.h CCD/ArmSegment.cpp CCD/ArmSegment.h CCD/Ccd.cpp CCD/Ccd.h)

set(CMAKE_MODULE_PATH "/usr/local/Cellar/sfml/2.4.2_1/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(Eigen3 REQUIRED)
include_directories(EIGEN_INCLUDE_DIR)
find_package(SFML REQUIRED system window graphics network audio)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(RobotArm_CCD ${SFML_LIBRARIES})
endif()