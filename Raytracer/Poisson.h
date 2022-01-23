#pragma once
#include "ShaderUtilities.h"
#include "graphics/GrPoint.h"
#include <set>
#include <ctime>

#pragma region Random Number Generator
// Random Numbers in C++ by: https://stackoverflow.com/questions/7560114/random-number-c-in-some-range

inline int random(int min, int max) //range : [min, max)
{
	static bool first = true;
	if (first)
	{
		srand(static_cast<long int>(time(nullptr))); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

inline double random(double min, double max)
{
	static bool first = true;
	if (first)
	{
		srand(static_cast<long int>(time(nullptr))); //seeding for the first time only!
		first = false;
	}
	double r = double(rand()) / double(RAND_MAX);
	return min + r * (max - min);
}

#pragma endregion

class CPoisson
{
public:
	CPoisson(double minimumD, double min = 0., double max = 1.) : m_minimumD(minimumD), m_min(min), m_max(max) {};
	
	shared_ptr<CGrPoint> Generate()
	{
		bool valid;
		shared_ptr<CGrPoint> p;
		do
		{
			valid = true; // Till we know otherwise
			p = make_shared<CGrPoint>(random(m_min, m_max), random(m_min, m_max));
			for (auto &item : m_history)
			{
				if (Distance(*item, *p) < m_minimumD)
				{
					valid = false;
				}
			}
		} while (!valid);
		m_history.insert(p);
		return p;
	}

	void Reset()
	{
		m_history.clear();
	}

private:
	std::set<shared_ptr<CGrPoint>> m_history;
	double m_min;
	double m_max;
	double m_minimumD;
};