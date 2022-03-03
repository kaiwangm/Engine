#pragma once

// pcl
#include <pcl/common/io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_cloud.h>

//
#include <Engine.h>

#include <iostream>

#include "Octree/Octree.h"

void save_ply(const std::vector<Engine::Ref<Engine::OctreeNode> >& levelnodes,
              const std::string& name) {
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_ptr =
        pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>);

    for (size_t i = 0; i < levelnodes.size(); ++i) {
        pcl::PointXYZ p;
        p.x = levelnodes[i]->GetX();
        p.y = levelnodes[i]->GetY();
        p.z = levelnodes[i]->GetZ();

        cloud_ptr->points.push_back(p);
    }
    std::cout << "Saving to file " << name << std::endl;
    pcl::io::savePLYFile(name, *cloud_ptr);
}

std::vector<std::array<uint32_t, 6> > load_ply(const std::string& name) {
    pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud(
        new pcl::PointCloud<pcl::PointXYZRGBNormal>);

    if (pcl::io::loadPLYFile<pcl::PointXYZRGBNormal>(name, *cloud) == -1) {
        PCL_ERROR("Couldn't read file. \n");
    }

    std::vector<std::array<uint32_t, 6> > points(cloud->points.size());
    for (int i = 0; i < cloud->points.size(); ++i) {
        points[i][0] = cloud->points[i].x;
        points[i][1] = cloud->points[i].y;
        points[i][2] = cloud->points[i].z;
        points[i][3] = cloud->points[i].r;
        points[i][4] = cloud->points[i].g;
        points[i][5] = cloud->points[i].b;
    }

    return points;
}