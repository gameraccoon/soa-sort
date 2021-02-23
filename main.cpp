#include <iostream>

#include <numeric>
#include <algorithm>
#include <vector>

using SomeData = float;
using SomeOtherData = float;
using SomeThirdData = float;

template <typename... ValueTypes>
struct SoAExample {
	std::vector<int> keys;
	std::tuple<std::vector<ValueTypes>...> values;
};

template<typename VectorType>
void applySwaps(std::vector<VectorType>& valuesVector, const std::vector<std::pair<size_t, size_t>>& swaps)
{
	for (auto [oldI, newI] : swaps)
	{
		std::swap(valuesVector[oldI], valuesVector[newI]);
	}
}

template<typename... ValueTypes>
void sortSoaVariant1(SoAExample<ValueTypes...>& inOutStruct)
{
	std::vector<size_t> sortedPositions;
	sortedPositions.resize(inOutStruct.keys.size());
	std::iota(sortedPositions.begin(), sortedPositions.end(), 0);

	std::sort(sortedPositions.begin(), sortedPositions.end(),
		[&inOutStruct](size_t i, size_t j) {
			return inOutStruct.keys[i] < inOutStruct.keys[j];
		}
	);

	// get reverse mapping
	std::vector<size_t> reverseMapping;
	reverseMapping.resize(sortedPositions.size());
	for (int i = 0; i < sortedPositions.size(); ++i)
	{
		reverseMapping[sortedPositions[i]] = i;
	}

	std::vector<std::pair<size_t, size_t>> swaps;
	swaps.reserve(sortedPositions.size());

	for (int i = 0; i < reverseMapping.size();)
	{
		size_t newI = reverseMapping[i];
		if (i != newI)
		{
			swaps.emplace_back(i, newI);
			reverseMapping[i] = reverseMapping[newI];
			reverseMapping[newI] = newI;
		}
		else
		{
			++i;
		}
	}

	applySwaps(inOutStruct.keys, swaps);

	std::apply(
		[&swaps](auto&... values)
		{
			(applySwaps(values, swaps), ...);
		},
		inOutStruct.values
	);
}

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

int main() {
	SoAExample<float, float, float> soaExample;
	fillSoa(soaExample);

	sortSoaVariant1(soaExample);

	printSoa(soaExample);

	return 0;
}
