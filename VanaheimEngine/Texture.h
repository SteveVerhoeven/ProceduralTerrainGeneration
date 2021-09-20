#pragma once
#include "Observer.h"

class Texture final : public Observer
{
	public:
		Texture(const std::string& filePath);
		~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture(Texture&&) = delete;
		Texture& operator=(Texture&&) = delete;

		void Initialize(const std::string& filePath);

		virtual void onNotify(ObserverEvent event) override;

		ID3D11ShaderResourceView* GetShaderResourceView() const { return m_pShaderResourceView; };

	private:
		std::string m_Path;
		ID3D11ShaderResourceView* m_pShaderResourceView;

		void LoadImageFromFile(DirectX::ScratchImage* pImage, DirectX::TexMetadata& info, const std::string& filePath);
};