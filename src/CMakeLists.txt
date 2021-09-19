add_library(RayTracer sphere.cpp scene.cpp plane.cpp)

target_link_libraries(RayTracer linalg)
target_include_directories(RayTracer PUBLIC ../include/src)