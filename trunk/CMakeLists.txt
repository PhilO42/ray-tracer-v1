CMAKE_MINIMUM_REQUIRED(VERSION 2.7)
PROJECT(ray_tracer)
SET(ray_tracer_VERSION_MAJOR 0)
SET(ray_tracer_VERSION_MINOR 1)
SET(CMAKE_BUILD_TYPE Debug)

SET(SRC_PATH src)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ray_tracer_SOURCE_DIR}/bin)


#qt
FIND_PACKAGE(Qt4 REQUIRED)
SET(ray_tracer_SOURCES ${SRC_PATH}/Main.cpp ${SRC_PATH}/RayTracer.cpp ${SRC_PATH}/Viewer.cpp ${SRC_PATH}/mathe/NMath.cpp ${SRC_PATH}/AABB.cpp ${SRC_PATH}/BVH.cpp ${SRC_PATH}/Light.cpp ${SRC_PATH}/Ray.cpp ${SRC_PATH}/SceneGraph.cpp ${SRC_PATH}/Sphere.cpp ${SRC_PATH}/Triangle.cpp ${SRC_PATH}/myUtil.cpp ${SRC_PATH}/Box.cpp ${SRC_PATH}/Plane.cpp ${SRC_PATH}/Cylinder.cpp)
SET(ray_tracer_HEADERS ${SRC_PATH}/RayTracer.h ${SRC_PATH}/Viewer.h ${SRC_PATH}/mathe/CMatrix.h ${SRC_PATH}/mathe/CTensor.h ${SRC_PATH}/mathe/CTensor4D.h ${SRC_PATH}/mathe/CVector.h ${SRC_PATH}/mathe/NMath.h ${SRC_PATH}/AABB.h ${SRC_PATH}/BVH.h ${SRC_PATH}/Light.h ${SRC_PATH}/Ray.h ${SRC_PATH}/SceneGraph.h ${SRC_PATH}/SceneObject.h ${SRC_PATH}/Sphere.h ${SRC_PATH}/Triangle.h ${SRC_PATH}/myUtil.h ${SRC_PATH}/Box.h  ${SRC_PATH}/Plane.h ${SRC_PATH}/Cylinder.h)
QT4_WRAP_CPP(ray_tracer_HEADERS_MOC ${ray_tracer_HEADERS})
INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

include_directories(
    ${ray_tracer_SOURCE_DIR}/src
)

ADD_EXECUTABLE(ray_tracer ${ray_tracer_SOURCES} ${ray_tracer_HEADERS_MOC})
TARGET_LINK_LIBRARIES(ray_tracer ${QT_LIBRARIES})
