#include <iostream>

#include "soasort.h"

template <typename... ValueTypes>
struct SoAExample {
	std::vector<int> keys;
	std::tuple<std::vector<ValueTypes>...> values;
};

void fillSoa(SoAExample<float, float, float>& outSoaExample)
{
	outSoaExample.keys = {1, 2, 3, 0};
	std::get<0>(outSoaExample.values) = {1.0f, 2.0f, 3.0f, 4.0f};
	std::get<1>(outSoaExample.values) = {2.0f, 3.0f, 0.0f, 1.0f};
	std::get<2>(outSoaExample.values) = {-3.0f, -2.0f, -1.0f, 0.0f};
}

void printSoa(const SoAExample<float, float, float>& soaExample)
{
	for (size_t i = 0; i < soaExample.keys.size(); ++i)
	{
		std::cout << "{key=\"" << soaExample.keys[i];
		std::cout << "\", v1=\"" << std::get<0>(soaExample.values)[i];
		std::cout << "\", v2=\"" << std::get<1>(soaExample.values)[i];
		std::cout << "\", v3=\"" << std::get<2>(soaExample.values)[i];
		std::cout << "\"}\n";
	}
}

int main()
{
	SoAExample<float, float, float> soaExample;
	fillSoa(soaExample);

	soasort::sortWithSwaps
	(
		soaExample.keys,
		std::get<0>(soaExample.values),
		std::get<1>(soaExample.values),
		std::get<2>(soaExample.values)
	);

	printSoa(soaExample);

	return 0;
}
