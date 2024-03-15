#pragma once
#include <functional>
#include <glm/glm.hpp>
#include <vector>

typedef std::vector<unsigned char> color;
using PickCallback = std::function<color(
    const glm::mat4 &p, const glm::mat4 &mv, int mouse_x, int mouse_y)>;
