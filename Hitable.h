#pragma once

#include "Ray.h"

struct HitRecord
{
	float t;
	Vector3 P;
	Vector3 N;
};

class Hitable
{
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};