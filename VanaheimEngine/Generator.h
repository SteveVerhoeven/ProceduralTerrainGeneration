#pragma once
#include "Observer.h"

#include <vector>
#include "GeneratorSettings.h"

class Generator : public Observer
{
	public:
		Generator(const std::string& name);
		virtual ~Generator() = default;

		Generator(const Generator&) = delete;
		Generator(Generator&&) noexcept = delete;
		Generator& operator=(const Generator&) = delete;
		Generator& operator=(Generator&&) noexcept = delete;

	protected:
		virtual void onNotify(ObserverEvent event) override;
		void GenerateImage(const std::vector<std::vector<DirectX::XMFLOAT3>>& map, const std::string& filePath, const DirectX::XMFLOAT2& mapSize);
		void GenerateImage(const std::vector<std::vector<float>>& map, const std::string& filePath, const DirectX::XMFLOAT2& mapSize);

	private:
		std::string m_Name;
};