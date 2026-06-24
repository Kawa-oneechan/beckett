#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <json5pp.hpp>
#include "Tickable.h"
#include "Types.h"

struct Polygon
{
private:
	std::vector<glm::vec2> points;
	glm::vec4 aabb;
public:
	glm::vec2 operator[](size_t i) const { return points[i]; }
	size_t size() const { return points.size(); }
	void reserve(size_t i) { points.reserve(i); }
	void resize(size_t i) { points.resize(i); }
	void push_back(glm::vec2& point) { points.push_back(point); recalc(); }
	void emplace_back(glm::vec2&& point) { points.emplace_back(point); recalc(); }
	auto begin() { return points.begin(); }
	auto end() { return points.end(); }
	auto cbegin() { return points.cbegin(); }
	auto cend() { return points.cend(); }
	void clear() { points.clear(); }
	bool empty() { return points.empty(); }
	void recalc();
	glm::vec4 AABB() { return aabb; };
};

//Returns true if point is inside of polygon.
extern bool PointInPoly(const glm::vec2 point, Polygon& polygon);
//Returns true if point is in rect.
extern bool PointInRect(const glm::vec2 point, const glm::vec4 rect);
