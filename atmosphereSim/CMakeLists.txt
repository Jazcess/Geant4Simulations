cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(AtmosphereSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

file(GLOB MACRO_FILES
	"*.mac"
)

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
	"*.dat"
)

file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(atmosphereSim atmosphereSim.cc ${sources} ${headers})
target_link_libraries(atmosphereSim ${Geant4_LIBRARIES})

add_custom_target(AtmosphereSim DEPENDS atmosphereSim)

