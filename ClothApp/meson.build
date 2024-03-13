glew_dep = dependency('glew')
freeglut_dep = dependency('glut')
glm_dep = dependency('glm')
openmesh_dep = dependency('openmesh')
eigen_dep = dependency('eigen3')
gl_dep = dependency('gl')
# glu_dep = dependency('glu')
cc = meson.get_compiler('c')
glu_dep = cc.find_library('glu32', required: true)

executable(
    'ClothApp',
    [
        'main.cpp',
        'app.cpp',
        'MassSpringSolver.cpp',
        'Mesh.cpp',
        'Renderer.cpp',
        'Shader.cpp',
        'UserInteraction.cpp',
    ],
    install: true,
    dependencies: [
        glew_dep,
        freeglut_dep,
        glm_dep,
        openmesh_dep,
        eigen_dep,
        gl_dep,
        glu_dep,
    ],
)
