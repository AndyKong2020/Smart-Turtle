cmake_minimum_required(VERSION 3.0.2)
project(yolo_identify_v5)

add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
add_definitions(-DENGINE_PATH="${PROJECT_SOURCE_DIR}")
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  image_transport
  rc_msgs
  roscpp
  std_msgs
  dynamic_reconfigure
)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

if (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    message("embed_platform on")
    include_directories(/usr/local/cuda/targets/aarch64-linux/include)
    link_directories(/usr/local/cuda/targets/aarch64-linux/lib)
else()
    message("embed_platform off")
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
    include_directories(/home/stevegao/TensorRT-7.2.3.4/include)
    link_directories(/home/stevegao/TensorRT-7.2.3.4/lib)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -g -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")
cuda_add_library(myplugins_identify SHARED src/yololayer.cu)
target_link_libraries(myplugins_identify nvinfer cudart)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES yolo_identify
#  CATKIN_DEPENDS cv_bridge image_transport rc_msgs roscpp std_msgs
#  DEPENDS system_lib
)
include_directories(
         include
        ${catkin_INCLUDE_DIRS}
)

cuda_add_executable(yolo_identify_v5_node src/calibrator.cpp src/yolov5.cpp src/preprocess.cu)
add_dependencies(yolo_identify_v5_node rc_msgs_generate_messages)

target_link_libraries(yolo_identify_v5_node nvinfer)
target_link_libraries(yolo_identify_v5_node cudart)
target_link_libraries(yolo_identify_v5_node myplugins_identify)
target_link_libraries(yolo_identify_v5_node ${catkin_LIBRARIES})
target_link_libraries(yolo_identify_v5_node ${OpenCV_LIBS})

if(UNIX)
    add_definitions(-O2 -pthread)
endif(UNIX)
