spring_deps = [
    dependency('glm'),
    dependency('eigen3'),
]
spring_lib = static_library(
    'MassSpringSystem',
    [
        'MassSpringSolver.cpp',
        'MassSpringBuilder.cpp',
        'ConstraintGraph.cpp',
    ],
    dependencies: spring_deps,
)
spring_dep = declare_dependency(
    link_with: spring_lib,
    include_directories: include_directories('.'),
    dependencies: spring_deps,
)
