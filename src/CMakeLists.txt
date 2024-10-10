add_executable(learnopengl
    Main.cpp Shader.cpp) 
target_compile_features(learnopengl PRIVATE cxx_std_23)
set_target_properties(learnopengl PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(learnopengl PUBLIC .)
target_link_sdl2(learnopengl)
target_link_libraries(learnopengl PRIVATE glad)
target_link_libraries(learnopengl PRIVATE ${OPENGL_LIBRARIES})

if (ENABLE_DEVELOPER_MODE)
    message("Setting developer mode compiler flags")
    target_compile_options(learnopengl PRIVATE -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Weffc++)
endif()
