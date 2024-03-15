project('openmesh', 'cpp')

openmesh_inc = include_directories('src')

openmesh_lib = static_library(
    'openmesh',
    [
        'src/OPenMesh/Core/IO/BinaryHelper.cc',
        'src/OPenMesh/Core/IO/IOManager.cc',
        'src/OPenMesh/Core/IO/OMFormat.cc',
        'src/OPenMesh/Core/IO/reader/BaseReader.cc',
        'src/OPenMesh/Core/IO/reader/OBJReader.cc',
        'src/OPenMesh/Core/IO/reader/OFFReader.cc',
        'src/OPenMesh/Core/IO/reader/OMReader.cc',
        'src/OPenMesh/Core/IO/reader/PLYReader.cc',
        'src/OPenMesh/Core/IO/reader/STLReader.cc',
        'src/OPenMesh/Core/IO/writer/BaseWriter.cc',
        'src/OPenMesh/Core/IO/writer/OBJWriter.cc',
        'src/OPenMesh/Core/IO/writer/OFFWriter.cc',
        'src/OPenMesh/Core/IO/writer/OMWriter.cc',
        'src/OPenMesh/Core/IO/writer/PLYWriter.cc',
        'src/OPenMesh/Core/IO/writer/STLWriter.cc',
        'src/OPenMesh/Core/IO/writer/VTKWriter.cc',
        'src/OPenMesh/Core/Mesh/ArrayKernel.cc',
        'src/OPenMesh/Core/Mesh/BaseKernel.cc',
        'src/OPenMesh/Core/Mesh/PolyConnectivity.cc',
        'src/OPenMesh/Core/Mesh/TriConnectivity.cc',
        'src/OPenMesh/Core/System/omstream.cc',
        'src/OPenMesh/Core/Utils/BaseProperty.cc',
        'src/OPenMesh/Core/Utils/Endian.cc',
        'src/OPenMesh/Core/Utils/PropertyCreator.cc',
        'src/OPenMesh/Core/Utils/RandomNumberGenerator.cc',
    ],
    include_directories: openmesh_inc,
    cpp_args: [
        '-D_USE_MATH_DEFINES',
        '/bigobj',
    ],
)

openmesh_dep = declare_dependency(
    include_directories: openmesh_inc,
)
