add_library(RayTracer sphere.cpp scene.cpp plane.cpp light.cpp rectangular-prism.cpp)

target_link_libraries(RayTracer linalg)
target_include_directories(RayTracer PUBLIC ../include/src)