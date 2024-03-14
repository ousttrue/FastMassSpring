glew_dep = dependency('glew')

# freeglut_dep = dependency('glut')
glfw_dep = dependency('glfw3', default_options: ['install=true'])

glm_dep = dependency('glm')
openmesh_dep = dependency('openmesh')
eigen_dep = dependency('eigen3')
gl_dep = dependency('gl')
# glu_dep = dependency('glu')
cc = meson.get_compiler('c')
glu_dep = cc.find_library('glu32', required: true)
imgui_dep = dependency('imgui', default_options: ['default_library=static'])

executable(
    'ClothApp',
    [
        'main.cpp',
        'app.cpp',
        'MassSpringSolver.cpp',
        'Mesh.cpp',
        'Shader.cpp',
        'Vao.cpp',
        'UserInteraction.cpp',
        'demo_base.cpp',
        'demo_drop.cpp',
        'demo_hang.cpp',
    ],
    install: true,
    dependencies: [
        glew_dep,
        # freeglut_dep,
        glfw_dep,
        glm_dep,
        openmesh_dep,
        eigen_dep,
        gl_dep,
        glu_dep,
        imgui_dep,
    ],
)
