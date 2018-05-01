#pragma once

#include "Vector3.h"

namespace Helper
{
	Vector3 LerpVector(const Vector3& vec1, const Vector3& vec2, float t)
	{
		return (1.0f - t) * vec1 + t * vec2;
	}

	double GetRandom01()
	{
		return ((double)rand() / (RAND_MAX + 1));
	}

	Vector3 RandomInUnitSphere()
	{
		Vector3 P;

		do
		{
			P = 2.0f * Vector3(GetRandom01(), GetRandom01(), GetRandom01()) - Vector3(1, 1, 1);
		} while (P.squaredLength() >= 1.0f);

		return P;
	}

	Vector3 Reflect(const Vector3& v, const Vector3& n)
	{
		return v - 2 * dot(v, n) * n;
	}
}
