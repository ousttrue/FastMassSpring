cc = meson.get_compiler('c')

deps = [
    dependency('glew'),
    dependency('glm'),
    dependency('gl'),
    cc.find_library('glu32', required: true),
    dependency('openmesh'),
]

renderer_lib = static_library(
    'Renderer',
    [
        'Shader.cpp',
        'Vao.cpp',
        'Mesh.cpp',
        'app.cpp',
    ],
    dependencies: deps,
)

renderer_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: renderer_lib,
    dependencies: deps,
)
