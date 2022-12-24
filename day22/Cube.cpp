#include "Cube.h"

#include <nlohmann/json.hpp>

#include <algorithm>

using json = nlohmann::json;

Cube::Cube(int minX, int maxX, int minY, int maxY, int minZ, int maxZ)
    : minX_(minX), maxX_(maxX)
    , minY_(minY), maxY_(maxY)
    , minZ_(minZ), maxZ_(maxZ)
{
    assert(!isEmpty());
}

bool Cube::isEmpty() const
{
    return minX_ > maxX_ || minY_ > maxY_ || minZ_ > maxZ_;
}

bool Cube::contains(Cube const& other) const
{
    return minX_ <= other.minX_ && maxX_ >= other.maxX_
        && minY_ <= other.minY_ && maxY_ >= other.maxY_
        && minZ_ <= other.minZ_ && maxZ_ >= other.maxZ_;
}

bool Cube::intersects(Cube const& other) const
{
    return ::intersects(*this, other);
}

int64_t Cube::size() const
{
    return (int64_t)(maxX_ - minX_ + 1) * (int64_t)(maxY_ - minY_ + 1) * (int64_t)(maxZ_ - minZ_ + 1);
}

bool intersects(Cube const& a, Cube const& b)
{
    return std::max(a.minX_, b.minX_) <= std::min(a.maxX_, b.maxX_)
        && std::max(a.minY_, b.minY_) <= std::min(a.maxY_, b.maxY_)
        && std::max(a.minZ_, b.minZ_) <= std::min(a.maxZ_, b.maxZ_);
}

Cube intersection(Cube const& a, Cube const& b)
{
    int minX = std::max(a.minX_, b.minX_);
    int maxX = std::min(a.maxX_, b.maxX_);
    int minY = std::max(a.minY_, b.minY_);
    int maxY = std::min(a.maxY_, b.maxY_);
    int minZ = std::max(a.minZ_, b.minZ_);
    int maxZ = std::min(a.maxZ_, b.maxZ_);

    return Cube(minX, maxX, minY, maxY, minZ, maxZ);
}

void to_json(json& j, Cube const& r)
{
    j = json::array({
        json::array({ r.minX_, r.minY_, r.minZ_ }),
        json::array({ r.maxX_, r.maxY_, r.maxZ_ })
        });
}
