#pragma once
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <vector>

using IndexedBuffer =
    std::tuple<std::shared_ptr<OpenMesh::TriMesh_ArrayKernelT<>>,
               std::vector<uint32_t>>;

IndexedBuffer uniformGrid(float w, int n);
