#pragma once

#include <Engine.h>

#include <iostream>

#include "happly.h"

namespace Engine {
void save_ply(const std::vector<Engine::Ref<Engine::OctreeNode>>& levelnodes,
              const std::string& name);

std::tuple<std::vector<std::array<int, 3>>,
           std::vector<std::array<uint32_t, 3>>>
load_ply(const std::string& name);
}