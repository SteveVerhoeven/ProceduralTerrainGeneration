#pragma once
#include <vector>
#include <string>

class ImageGenerator final
{
	public:
		ImageGenerator() = default;
		~ImageGenerator() = default;

		ImageGenerator(const ImageGenerator&) = delete;
		ImageGenerator(ImageGenerator&&) noexcept = delete;
		ImageGenerator& operator=(const ImageGenerator&) = delete;
		ImageGenerator& operator=(ImageGenerator&&) noexcept = delete;

		static void GenerateImage(const std::vector<std::vector<DirectX::XMFLOAT3>>& map, const std::string& filePath, const DirectX::XMINT2& mapSize);
		static void GenerateImage(const std::vector<std::vector<float>>& map, const std::string& filePath, const DirectX::XMINT2& mapSize);

	protected:

	private:

};