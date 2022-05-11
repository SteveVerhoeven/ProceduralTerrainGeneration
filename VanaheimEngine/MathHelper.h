#pragma once
#include "VanaheimPCH.h"

#include <algorithm>

inline int RandomInt(const int min, const int max)
{
	return min + (rand() % (max - min + 1));
}

template<typename T>
T Remap(T val, T min, T max)
{
	return (val - min) / (max - min);
}

template<typename T>
T Normalize1DVector(T vector)
{
	// Find out what the range is of the current values in the vector
	float minHeight{};
	float maxHeight{};
	const size_t vectorSize{ vector.size() };
	for (size_t i{}; i < vectorSize; ++i)
	{
		const float value{ vector[i] };
		if (value > maxHeight)
			maxHeight = value;
		else if (value < minHeight)
			minHeight = value;
	}

	// Remap the values from the old range to the [0, 1] range
	const size_t vectorSize{ vector.size() };
	for (size_t i{}; i < vectorSize; ++i)
		vector = Remap<float>(minHeight, maxHeight, 0.f, 1.f, vector[i]);
}
template<typename T>
std::vector<std::vector<T>> Normalize2DVector(std::vector<std::vector<T>> vector)
{
	// Find out what the range is of the current values in the vector
	float minHeight{ FLT_MAX };
	float maxHeight{ -FLT_MAX };
	const size_t vectorSize{ vector[0].size() };
	for (size_t x{}; x < vectorSize; ++x)
	{
		// Find the lowest value
		auto res1{ std::min_element(vector[x].begin(), vector[x].end()) };
		const float min{ vector[x][std::distance(vector[x].begin(), res1)] };
		minHeight = (min < minHeight) ? min : minHeight ;

		// Find the highest value
		auto res2{ std::max_element(vector[x].begin(), vector[x].end()) };
		const float max{ vector[x][std::distance(vector[x].begin(), res2)] };
		maxHeight = (max > maxHeight) ? max : maxHeight;
	}

	// Remap the values from the old range[minHeight,maxHeight] to the [0, 1] range
	for (size_t y{}; y < vectorSize; ++y)
		for (size_t x{}; x < vectorSize; ++x)
			vector[x][y] = Remap(vector[x][y], minHeight, maxHeight);
			//vector[x][y] = Remap(vector[x][y], 0.f, 1.f);

	return vector;
}

inline float ConvertBytesToKiloBytes(const unsigned int bytes)
{
	return bytes / 1024.f;
}
inline float ConvertBytesToMegaBytes(const unsigned int bytes)
{
	return ConvertBytesToKiloBytes(bytes) / 1024.f;
}
inline float ConvertBytesToGigaBytes(const unsigned int bytes)
{
	return ConvertBytesToMegaBytes(bytes) / 1024.f;
}

inline float ConvertNanoSecondsToMilliSeconds(const long long nanoSeconds)
{
	return nanoSeconds * powf(10, -6);
}
inline float ConvertMicroSecondsToMilliSeconds(const long long nanoSeconds)
{
	return nanoSeconds * powf(10, -3);
}

inline bool EqualFloat3(const DirectX::XMFLOAT3& point1, const DirectX::XMFLOAT3& point2)
{
	if (abs(point1.x - point2.x) < FLT_EPSILON)
		if (abs(point1.y - point2.y) < FLT_EPSILON)
			if (abs(point1.z - point2.z) < FLT_EPSILON)
				return true;

	return false;
}
inline bool EqualFloat(const float value1, const float value2)
{
	if (abs(value1 - value2) < FLT_EPSILON)
		return true;

	return false;
}