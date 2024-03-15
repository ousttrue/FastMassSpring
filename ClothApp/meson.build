cloth_lib = static_library(
    'ClothApp',
    [
        'MassSpringSolver.cpp',
        'UserInteraction.cpp',
        'demo_base.cpp',
        'demo_drop.cpp',
        'demo_hang.cpp',
    ],
    dependencies: [
        dependency('glm'),
        dependency('eigen3'),
    ],
)

cloth_dep = declare_dependency(
    include_directories: include_directories('.'),
    link_with: cloth_lib,
)
