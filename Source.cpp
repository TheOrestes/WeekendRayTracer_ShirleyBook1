
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

Hitable* random_scene()
{
	int n = 500;
	Hitable** list = new Hitable*[n + 1];
	list[0] = new Sphere(Vector3(0, -1000, 0), 1000, new Lambertian(Vector3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choose_mat = Helper::GetRandom01();
			Vector3 center(a + 0.9f*Helper::GetRandom01(), 0.2, b + 0.9*Helper::GetRandom01());
			if ((center - Vector3(4, 0.2, 0)).length() > 0.9f)
			{
				if (choose_mat < 0.8f)
				{
					// diffuse
					list[i++] = new Sphere(center, 0.2f, new Lambertian(Vector3(Helper::GetRandom01()*Helper::GetRandom01(), Helper::GetRandom01()*Helper::GetRandom01(), Helper::GetRandom01()*Helper::GetRandom01())));
				}
				else if (choose_mat < 0.95)
				{
					// Metal
					list[i++] = new Sphere(center, 0.2f, new Metal(Vector3(0.5f*(1 + Helper::GetRandom01()), 0.5f*(1 + Helper::GetRandom01()), 0.5f*(1 + Helper::GetRandom01())), Helper::GetRandom01()));
				}
				else
				{
					// glass
					list[i++] = new Sphere(center, 0.2f, new Transparent(1.5f));
				}
			}
		}
	}

	list[i++] = new Sphere(Vector3(0, 1, 0), 1.0f, new Transparent(1.5f));
	list[i++] = new Sphere(Vector3(-4, 1, 0), 1.0f, new Lambertian(Vector3(0.4f, 0.2f, 0.1f)));
	list[i++] = new Sphere(Vector3(4, 1, 0), 1.0f, new Metal(Vector3(0.7f, 0.6f, 0.5f), 0.0f));

	return new HitableList(list, i);
}

Hitable* BasicTestScene()
{
	Hitable** list = new Hitable*[5];
	list[0] = new Sphere(Vector3(1.05f, 0, 0), 0.5, new Metal(Vector3(0.5, 0.2, 0.1), 0.5));
	list[1] = new Sphere(Vector3(0, -100.5, 0), 100, new Lambertian(Vector3(0.2, 0.2, 0.2)));
	list[2] = new Sphere(Vector3(0, 0, 0.1), 0.5, new Transparent(1.5f));
	list[3] = new Sphere(Vector3(-1.05f, 0, 0), 0.5, new Metal(Vector3(1.0, 0.2, 0.0), 0));
	list[4] = new Sphere(Vector3(0.0f, 0, -3), 0.5, new Lambertian(Vector3(1.0, 1.0, 0.0)));

	return new HitableList(list, 5);
}

int main()
{
	FILE* filePtr = std::fopen("rayTracer.ppm", "w");
	if (!filePtr)
		return 0;

	int nx = 400;
	int ny = 200;
	int ns = 1;	// number of samples per pixel!

	fprintf(filePtr, "P3\n %d %d \n255", nx, ny);
	
	
	Hitable* world = BasicTestScene();

	Vector3 lookFrom(0,1.5,6);
	Vector3 lookAt(0, 0, 0);
	float dist_to_focus = 1.0f;	// set this to 1.0 & apertue to 0.0f to stop DOF effect!
	float aperture = 0.0f;

	Camera cam(lookFrom, lookAt, Vector3(0,1,0), 20, float(nx) / float(ny), aperture, dist_to_focus );
	int percentageDone = 0.0f;

	const clock_t begin_time = clock();
	double counter = 0;

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

			++counter;
		}

		percentageDone = (counter / (nx*ny)) * 100.0f;
		ShowProgress(percentageDone);
	}

	const clock_t end_time = clock();
	double time = (end_time - begin_time) / (double)CLOCKS_PER_SEC;

	printf("Render Time : %.2f seconds\n", time);

	system("pause");

	return 1;
}