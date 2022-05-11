#pragma once
#include <vector>

class Image final
{
	public:
		Image(const DirectX::XMINT2& dimensions);
		virtual ~Image() = default;

		Image(const Image&) = delete;
		Image(Image&&) noexcept = delete;
		Image& operator=(const Image&) = delete;
		Image& operator=(Image&&) noexcept = delete;

		void ExportImage(const std::string& path);
		
		DirectX::XMFLOAT3 GetColor(const size_t x, const size_t y) const { return m_Colors[size_t(y * m_Dimensions.x + x)]; }
		void SetColor(const DirectX::XMFLOAT3& color, const size_t x, const size_t y) { m_Colors[size_t(y * m_Dimensions.x + x)] = color; }
		void SetColor(const std::vector<std::vector<float>>& noiseMap);
		void SetColor(const std::vector<std::vector<DirectX::XMFLOAT3>>& noiseMap);

	protected:
	private:
		DirectX::XMINT2 m_Dimensions;
		std::vector<DirectX::XMFLOAT3> m_Colors;
};