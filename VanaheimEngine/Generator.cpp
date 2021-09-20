#include "pch.h"
#include "Generator.h"

#include "Image.h"

Generator::Generator(const std::string& name)
		  : m_Name(name)
{}

void Generator::onNotify(ObserverEvent /*event*/)
{
}

void Generator::GenerateImage(const std::vector<std::vector<float>>& map, const std::string& filePath, const DirectX::XMFLOAT2& mapSize)
{
	Image image({ mapSize.x, mapSize.y });
	image.SetColor(map);
	image.ExportImage(filePath);
}
void Generator::GenerateImage(const std::vector<std::vector<DirectX::XMFLOAT3>>& map, const std::string& filePath, const DirectX::XMFLOAT2& mapSize)
{
	Image image({ mapSize.x, mapSize.y });
	image.SetColor(map);
	image.ExportImage(filePath);
}