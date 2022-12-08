cmake_minimum_required(VERSION 3.0.2)
project(serial_port)

find_package(
        catkin REQUIRED COMPONENTS
        geometry_msgs
        roscpp
        serial
        std_msgs
        message_generation
)
 add_message_files(
        FILES
        text.msg
        #   Message2.msg
 )
generate_messages(
        DEPENDENCIES
        std_msgs
)
catkin_package(
        #  INCLUDE_DIRS include
        #  LIBRARIES serial_port
          CATKIN_DEPENDS geometry_msgs roscpp serial std_msgs message_runtime
        #  DEPENDS system_lib
)

include_directories(
        include
        ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node src/main.cpp src/serialPort.cpp)

target_link_libraries(
        ${PROJECT_NAME}_node
        ${catkin_LIBRARIES}
)
