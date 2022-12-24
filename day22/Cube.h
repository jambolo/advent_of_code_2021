#if !defined(REGION_H_INCLUDED)
#define REGION_H_INCLUDED

#pragma once

#include <nlohmann/json_fwd.hpp>
#include <list>

class Cube
{
public:
    Cube(int minX, int maxX, int minY, int maxY, int minZ, int maxZ);
    bool isEmpty() const;
    bool contains(Cube const& other) const;
    bool intersects(Cube const& other) const;
    int64_t size() const;

    int minX_, maxX_;
    int minY_, maxY_;
    int minZ_, maxZ_;
};

bool intersects(Cube const& a, Cube const& b);
Cube intersection(Cube const& a, Cube const& b);

void to_json(nlohmann::json& j, Cube const& r);

#endif // !defined(REGION_H_INCLUDED)