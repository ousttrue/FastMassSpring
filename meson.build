project('ClothApp', 'cpp')

add_project_arguments(
    '-DNOMINMAX',
    '-D_USE_MATH_DEFINES',
    language: 'cpp',
)

subdir('Renderer')
subdir('MassSpringSystem')
subdir('ClothApp')

executable(
    'ClothApp',
    'main.cpp',
    install: true,
    dependencies: [
        renderer_dep,
        cloth_dep,
        dependency('glfw3', default_options: ['install=true']),
        dependency('imgui', default_options: ['default_library=static']),
    ],
)
