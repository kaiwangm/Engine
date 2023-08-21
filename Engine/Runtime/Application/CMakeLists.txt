add_library(
    Application
    STATIC
    Application.cpp
    ImGuiLayer.cpp
    Window.cpp
    OpenGLImGuiLayer.cpp
    WindowsInput.cpp
)

target_link_libraries(
    Application
    PRIVATE
    Core
    Gui
    Renderer
)

target_link_libraries(
    Application
    PRIVATE
    glm::glm
    imgui::imgui
)

IF(ENGINE_PLATFORM STREQUAL "Windows")
    add_subdirectory(Windows_Window)
    target_link_libraries(
        Application
        PRIVATE
        Windows_Window
    )
ENDIF()

IF(ENGINE_OPENGL_USE)
    add_subdirectory(OpenGL_Imgui)
    target_link_libraries(
        Application
        PRIVATE
        Gui_OpenGL_Imgui
    )
ENDIF()

target_include_directories(Application INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
