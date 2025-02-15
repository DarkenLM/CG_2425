#pragma once

void saveToFile(const std::string& filename, uint8_t version, std::vector<Point3D>& triangles);
void commonTest();