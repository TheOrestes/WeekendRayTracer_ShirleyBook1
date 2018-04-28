
#include <iostream>
#include <stdio.h>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

const int COLOR_CHANNELS = 3;	// RGB

Vector3 LerpVector(const Vector3& vec1, const Vector3& vec2, float t)
{
	return (1.0f - t) * vec1 + t * vec2;
}



Vector3 TraceColor(const Ray& r, Hitable* world)
{
	HitRecord rec;

	if (world->hit(r, 0.0f, FLT_MAX, rec))
	{
		return 0.5f * Vector3(rec.N.x + 1, rec.N.y + 1, rec.N.z + 1);
	}
	else
	{
		Vector3 unit_direction = unit_vector(r.GetRayDirection());
		float t = 0.5 * (unit_direction.y + 1.0f);
		return LerpVector(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.7f, 1.0f), t);
	}
}

int GetRandom01()
{
	return ((double)rand() / (RAND_MAX)) + 1;
}

int main()
{
	FILE* filePtr = std::fopen("rayTracer.ppm", "w");
	if (!filePtr)
		return 0;

	int nx = 800;
	int ny = 400;
	int ns = 1;	// number of samples per pixel!

	fprintf(filePtr, "P3\n %d %d \n255", nx, ny);

	Vector3 origin(0.0f, 0.0f, 0.0f);
	Vector3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3 horizontal(4.0f, 0.0f, 0.0f);
	Vector3 vertical(0.0f, 2.0f, 0.0f);

	Hitable* list[2];
	list[0] = new Sphere(Vector3(0, -100.5, -1), 100);
	list[1] = new Sphere(Vector3(0, 0, -1), 0.5);

	Hitable* world = new HitableList(list, 2);

	Camera cam;

	for (int j = ny-1 ; j >= 0 ; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vector3 color(0, 0, 0);

			for (int s = 0; s < ns; s++)
			{
				float u = float(i + GetRandom01()) / float(nx);
				float v = float(j + GetRandom01()) / float(ny);

				Ray r = cam.get_ray(u, v);

				color = color + TraceColor(r, world);
			}

			color = color / float(ns);

			int ir = int(255.99*color.x);
			int ig = int(255.99*color.y);
			int ib = int(255.99*color.z);

			fprintf(filePtr, "\n%d %d %d", ir, ig, ib);
		}
	}

	return 1;
}