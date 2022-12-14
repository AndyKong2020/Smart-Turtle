# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "rc_msgs: 4 messages, 0 services")

set(MSG_I_FLAGS "-Irc_msgs:/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg;-Isensor_msgs:/opt/ros/noetic/share/sensor_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/noetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(rc_msgs_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_custom_target(_rc_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rc_msgs" "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" ""
)

get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_custom_target(_rc_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rc_msgs" "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" "rc_msgs/point"
)

get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_custom_target(_rc_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rc_msgs" "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" "std_msgs/Header:rc_msgs/detection:rc_msgs/point:sensor_msgs/PointField:sensor_msgs/PointCloud2:sensor_msgs/Image"
)

get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_custom_target(_rc_msgs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "rc_msgs" "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" "rc_msgs/point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
)
_generate_msg_cpp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
)
_generate_msg_cpp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointField.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointCloud2.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
)
_generate_msg_cpp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
)

### Generating Services

### Generating Module File
_generate_module_cpp(rc_msgs
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(rc_msgs_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(rc_msgs_generate_messages rc_msgs_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_cpp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_cpp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_cpp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_cpp _rc_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rc_msgs_gencpp)
add_dependencies(rc_msgs_gencpp rc_msgs_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rc_msgs_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
)
_generate_msg_eus(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
)
_generate_msg_eus(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointField.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointCloud2.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
)
_generate_msg_eus(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
)

### Generating Services

### Generating Module File
_generate_module_eus(rc_msgs
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(rc_msgs_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(rc_msgs_generate_messages rc_msgs_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_eus _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_eus _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_eus _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_eus _rc_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rc_msgs_geneus)
add_dependencies(rc_msgs_geneus rc_msgs_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rc_msgs_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
)
_generate_msg_lisp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
)
_generate_msg_lisp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointField.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointCloud2.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
)
_generate_msg_lisp(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
)

### Generating Services

### Generating Module File
_generate_module_lisp(rc_msgs
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(rc_msgs_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(rc_msgs_generate_messages rc_msgs_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_lisp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_lisp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_lisp _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_lisp _rc_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rc_msgs_genlisp)
add_dependencies(rc_msgs_genlisp rc_msgs_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rc_msgs_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
)
_generate_msg_nodejs(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
)
_generate_msg_nodejs(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointField.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointCloud2.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
)
_generate_msg_nodejs(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
)

### Generating Services

### Generating Module File
_generate_module_nodejs(rc_msgs
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(rc_msgs_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(rc_msgs_generate_messages rc_msgs_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_nodejs _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_nodejs _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_nodejs _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_nodejs _rc_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rc_msgs_gennodejs)
add_dependencies(rc_msgs_gennodejs rc_msgs_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rc_msgs_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
)
_generate_msg_py(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
)
_generate_msg_py(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/std_msgs/cmake/../msg/Header.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg;/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointField.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/PointCloud2.msg;/opt/ros/noetic/share/sensor_msgs/cmake/../msg/Image.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
)
_generate_msg_py(rc_msgs
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg"
  "${MSG_I_FLAGS}"
  "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
)

### Generating Services

### Generating Module File
_generate_module_py(rc_msgs
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(rc_msgs_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(rc_msgs_generate_messages rc_msgs_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/point.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_py _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/detection.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_py _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/results.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_py _rc_msgs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/stevegao/CLionProjects/Smart-Turtle/Smart-Turtle-Client/src/rc_msgs/msg/calibrateResult.msg" NAME_WE)
add_dependencies(rc_msgs_generate_messages_py _rc_msgs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(rc_msgs_genpy)
add_dependencies(rc_msgs_genpy rc_msgs_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS rc_msgs_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/rc_msgs
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(rc_msgs_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/rc_msgs
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(rc_msgs_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/rc_msgs
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(rc_msgs_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/rc_msgs
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(rc_msgs_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/rc_msgs
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(rc_msgs_generate_messages_py sensor_msgs_generate_messages_py)
endif()
