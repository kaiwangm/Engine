#pragma once

//
#include <Engine.h>

#include <iostream>

#include "happly.h"

void save_ply(const std::vector<Engine::Ref<Engine::OctreeNode>>& levelnodes,
              const std::string& name) {}

std::tuple<std::vector<std::array<int, 3>>,
           std::vector<std::array<uint32_t, 3>>>
load_ply(const std::string& name) {
    happly::PLYData plyIn(name);
    auto x = plyIn.getElement("vertex").getProperty<float>("x");
    auto y = plyIn.getElement("vertex").getProperty<float>("y");
    auto z = plyIn.getElement("vertex").getProperty<float>("z");
    auto r = plyIn.getElement("vertex").getProperty<unsigned char>("red");
    auto g = plyIn.getElement("vertex").getProperty<unsigned char>("green");
    auto b = plyIn.getElement("vertex").getProperty<unsigned char>("blue");

    std::vector<std::array<int, 3>> coords(x.size());
    std::vector<std::array<uint32_t, 3>> feats(x.size());

    for (int i = 0; i < coords.size(); ++i) {
        coords[i][0] = x[i];
        coords[i][1] = y[i];
        coords[i][2] = z[i];
        feats[i][0] = r[i];
        feats[i][1] = g[i];
        feats[i][2] = b[i];
    }

    return {coords, feats};
}