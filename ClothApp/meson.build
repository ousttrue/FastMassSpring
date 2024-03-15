cloth_lib = static_library(
    'ClothApp',
    [
        'app.cpp',
        'MassSpringSolver.cpp',
        'Mesh.cpp',
        'UserInteraction.cpp',
        'demo_base.cpp',
        'demo_drop.cpp',
        'demo_hang.cpp',
    ],
    dependencies: [
        dependency('glm'),
        dependency('openmesh'),
        dependency('eigen3'),
        renderer_dep,
    ],
)

cloth_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: cloth_lib,
)
