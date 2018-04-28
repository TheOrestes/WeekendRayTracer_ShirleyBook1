#pragma once

#include "Hitable.h"

class Sphere : public Hitable
{
public:
	Sphere() {}
	Sphere(Vector3 _center, float _r) :
		center(_center),
		radius(_r) {};

	virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const;

private:
	Vector3 center;
	float radius;
};

/////////////////////////////////////////////////////////////////////////////////////////
bool Sphere::hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const
{
	Vector3 oc = r.GetRayOrigin() - center;
	float a = dot(r.GetRayDirection(), r.GetRayDirection());
	float b = 2.0f * dot(oc, r.GetRayDirection());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a* c;

	float t;

	if (discriminant > 0)
	{
		t = (-b - sqrt(discriminant)) / (2.0 * a);
		if (t < tmax && t > tmin)
		{
			rec.t = t;
			rec.P = r.GetPointAt(t);
			rec.N = (rec.P - center) / radius;
			return true;
		}

		t = (-b + sqrt(discriminant)) / (2.0 * a);
		if (t < tmax && t > tmin)
		{
			rec.t = t;
			rec.P = r.GetPointAt(t);
			rec.N = (rec.P - center) / radius;
			return true;
		}
	}

	return false;
}