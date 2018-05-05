
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Helper.h"
#include "Material.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Transparent.h"

const int COLOR_CHANNELS = 3;	// RGB

Vector3 TraceColor(const Ray& r, Hitable* world, int depth)
{
	HitRecord rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scatteredRay;
		Vector3 attenuation;

		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scatteredRay))
		{
			return attenuation * TraceColor(scatteredRay, world, depth + 1);
		}
		else
		{
			return Vector3(0, 0, 0);
		}
	}
	else
	{
		Vector3 unit_direction = unit_vector(r.GetRayDirection());
		float t = 0.5 * (unit_direction.y + 1.0f);
		return Helper::LerpVector(Vector3(1.0f, 1.0f, 1.0f), Vector3(0.5f, 0.7f, 1.0f), t);
	}
}

void ShowProgress(int percentage)
{
	system("cls");
	printf("\nRendering Progress : %d%%\n", percentage);
}

int main()
{
	FILE* filePtr = std::fopen("rayTracer.ppm", "w");
	if (!filePtr)
		return 0;

	int nx = 800;
	int ny = 400;
	int ns = 100;	// number of samples per pixel!

	fprintf(filePtr, "P3\n %d %d \n255", nx, ny);

	Vector3 origin(0.0f, 0.0f, 0.0f);
	Vector3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vector3 horizontal(4.0f, 0.0f, 0.0f);
	Vector3 vertical(0.0f, 2.0f, 0.0f);

	Hitable* list[4];
	list[0] = new Sphere(Vector3(0, -100.5, -1), 100, new Lambertian(Vector3(0.2, 0.2, 0.2)));
	list[1] = new Sphere(Vector3(1, 0, -1.5), 0.5, new Metal(Vector3(0.8, 0.6, 0.2), 0.0));
	list[2] = new Sphere(Vector3(-1, 0, -1.5), 0.5, new Transparent(1.5f));
	list[3] = new Sphere(Vector3(0, 0, -1.25), 0.5, new Lambertian(Vector3(1.0, 0.2, 0.0)));
	Hitable* world = new HitableList(list, 4);

	Camera cam;
	int percentageDone = 0.0f;

	const clock_t begin_time = clock();

	for (int j = ny-1 ; j >= 0 ; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vector3 color(0, 0, 0);

			for (int s = 0; s < ns; s++)
			{
				float u = float(i + Helper::GetRandom01()) / float(nx);
				float v = float(j + Helper::GetRandom01()) / float(ny);

				Ray r = cam.get_ray(u, v);

				color = color + TraceColor(r, world, 0);
			}

			color = color / float(ns);
			color = Vector3(sqrt(color.x), sqrt(color.y), sqrt(color.z));

			int ir = int(255.99*color.x);
			int ig = int(255.99*color.y);
			int ib = int(255.99*color.z);

			fprintf(filePtr, "\n%d %d %d", ir, ig, ib);
		}

		percentageDone = ((ny - j)/4.0f);
		ShowProgress(percentageDone);
	}

	const clock_t end_time = clock();
	double time = (end_time - begin_time) / (double)CLOCKS_PER_SEC;

	printf("Render Time : %.2f seconds\n", time);

	system("pause");

	return 1;
}