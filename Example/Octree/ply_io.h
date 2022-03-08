#pragma once

//
#include <Engine.h>

#include <iostream>

#include "Octree/Octree.h"
#include "happly.h"

void save_ply(const std::vector<Engine::Ref<Engine::OctreeNode> >& levelnodes,
              const std::string& name) {}

std::vector<std::array<uint32_t, 6> > load_ply(const std::string& name) {
    happly::PLYData plyIn(name);
    auto x = plyIn.getElement("vertex").getProperty<float>("x");
    auto y = plyIn.getElement("vertex").getProperty<float>("y");
    auto z = plyIn.getElement("vertex").getProperty<float>("z");
    auto r = plyIn.getElement("vertex").getProperty<unsigned char>("red");
    auto g = plyIn.getElement("vertex").getProperty<unsigned char>("green");
    auto b = plyIn.getElement("vertex").getProperty<unsigned char>("blue");
    std::vector<std::array<uint32_t, 6> > points(x.size());

    for (int i = 0; i < points.size(); ++i) {
        points[i][0] = x[i];
        points[i][1] = y[i];
        points[i][2] = z[i];
        points[i][3] = r[i];
        points[i][4] = g[i];
        points[i][5] = b[i];
    }

    return points;
}