#pragma once

#include "Ray.h"

class Camera
{
public:
	Camera()
	{
		origin = Vector3(0, 0, 0);
		lower_left_corner = Vector3(-2.0f, -1.0f, -1.0f);
		horizontal = Vector3(4.0f, 0.0f, 0.0f);
		vertical = Vector3(0.0f, 2.0f, 0.0f);
	}

	Ray get_ray(float u, float v)
	{
		return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
	}

private:
	Vector3 origin;
	Vector3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
};
