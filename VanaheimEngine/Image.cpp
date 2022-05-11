#include "VanaheimPCH.h"
#include "Image.h"

Image::Image(const DirectX::XMINT2& dimensions)
	  : m_Dimensions(dimensions)
	  , m_Colors(std::vector<DirectX::XMFLOAT3>(size_t(dimensions.x * dimensions.y)))
{}

void Image::ExportImage(const std::string & path)
{
	// Reference: https://www.youtube.com/watch?v=vqT5j38bWGg
	// Reference: https://en.wikipedia.org/wiki/BMP_file_format
	std::ofstream fileStream;
	remove(path.c_str());
	fileStream.open(path.c_str(), std::ios::out | std::ios::binary);
	if (!fileStream.is_open())
	{
		LOG(ErrorLevel::LOG_FATAL, "File could not be openend");
		return;
	}

	unsigned char bmpPad[3]{ 0, 0, 0 };
	const int paddingAmount{ ((4 - (int(m_Dimensions.x) * 3) % 4) % 4) };

	const int fileHeaderSize{ 14 };
	const int informationHeaderSize{ 40 };
	const int fileSize{ int(fileHeaderSize + informationHeaderSize + m_Dimensions.x * m_Dimensions.y * 3 + paddingAmount * m_Dimensions.y) };

	unsigned char fileHeader[fileHeaderSize];
	// File type
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	// File size
	fileHeader[2] = unsigned char(fileSize);
	fileHeader[3] = unsigned char(fileSize >> 8);
	fileHeader[4] = unsigned char(fileSize >> 16);
	fileHeader[5] = unsigned char(fileSize >> 24);
	// Reserved 1 (Not used)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	// Reserved 1 (Not used)
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	// Pixel data offset
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];
	// Header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	// Image width
	informationHeader[4] = unsigned char(m_Dimensions.x);
	informationHeader[5] = unsigned char(int(m_Dimensions.x) >> 8);
	informationHeader[6] = unsigned char(int(m_Dimensions.x) >> 16);
	informationHeader[7] = unsigned char(int(m_Dimensions.x) >> 24);
	// Image height
	informationHeader[8]  = unsigned char(m_Dimensions.y);
	informationHeader[9]  = unsigned char(int(m_Dimensions.y) >> 8);
	informationHeader[10] = unsigned char(int(m_Dimensions.y) >> 16);
	informationHeader[11] = unsigned char(int(m_Dimensions.y) >> 24);
	// Planes
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	// Bits per pixel (RGB)
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	// Compression (No compression)
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	// Image size (No compression)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	// X pixels per meter (Not specified)
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	// Y pixels per meter (Not specified)
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	// Total colors (Color palette not used)
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	// Important colors (Generally ignored)
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	fileStream.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	fileStream.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	for (int y{}; y < m_Dimensions.y; ++y)
	{
		for (int x{}; x < m_Dimensions.x; ++x)
		{
			unsigned char r{ static_cast<unsigned char>(GetColor((size_t)x, (size_t)y).x * 255.f) };
			unsigned char g{ static_cast<unsigned char>(GetColor((size_t)x, (size_t)y).y * 255.f) };
			unsigned char b{ static_cast<unsigned char>(GetColor((size_t)x, (size_t)y).z * 255.f) };

			unsigned char color[] = { b, g, r };

			fileStream.write(reinterpret_cast<char*>(color), 3);
		}

		fileStream.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	fileStream.close();
}


void Image::SetColor(const std::vector<std::vector<float>>& noiseMap)
{
	for (int y{}; y < m_Dimensions.y; ++y)
	{
		for (int x{}; x < m_Dimensions.x; ++x)
		{
			const float value{ noiseMap[(size_t)y][(size_t)x] };

			DirectX::XMFLOAT3 color{};
			color.x = value;
			color.y = value;
			color.z = value;
			SetColor(color, (size_t)x, (size_t)y);
		}
	}
}
void Image::SetColor(const std::vector<std::vector<DirectX::XMFLOAT3>>& colorMap)
{
	for (int y{}; y < m_Dimensions.y; ++y)
	{
		for (int x{}; x < m_Dimensions.x; ++x)
		{
			const DirectX::XMFLOAT3 value{ colorMap[(size_t)y][(size_t)x] };
			SetColor(value, (size_t)x, (size_t)y);
		}
	}
}