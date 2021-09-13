add_executable(RayTracerApp app.cpp)

target_link_libraries(RayTracerApp raylib)
target_link_libraries(RayTracerApp RayTracer)
target_include_directories(RayTracerApp PUBLIC ../include/app)