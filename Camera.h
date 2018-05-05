#pragma once

#include "Ray.h"
#include "Helper.h"

class Camera
{
public:
	Camera(Vector3 lookFrom, Vector3 lookAt, Vector3 Up, float vfov, float aspect)	// vofv is vertical fov
	{
		Vector3 u, v, w;

		float theta = vfov * PI / 180.0f;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookFrom;
		w = unit_vector(lookFrom - lookAt);
		u = unit_vector(cross(Up, w));
		v = cross(w, u);

		lower_left_corner = Vector3(-half_width, -half_height, -1.0f);
		lower_left_corner = origin - half_width * u - half_height * v - w;
		horizontal = 2 * half_width * u;
		vertical = 2 * half_height * v;
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
