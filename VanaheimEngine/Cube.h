#pragma once
#include "Mesh_Base.h"

enum class CornerType
{
	TOP_LEFT_FRONT,
	TOP_RIGHT_FRONT,
	BOTTOM_LEFT_FRONT,
	BOTTOM_RIGHT_FRONT,

	TOP_LEFT_BACK,
	TOP_RIGHT_BACK,
	BOTTOM_LEFT_BACK,
	BOTTOM_RIGHT_BACK,
};

class Scene;
class Cube final : public Mesh_Base
{
	public:
		Cube() = default;
		~Cube() = default;

		Cube(const Cube&) = delete;
		Cube(Cube&&) noexcept = delete;
		Cube& operator=(const Cube&) = delete;
		Cube& operator=(Cube&&) noexcept = delete;

		void CreateLines(Scene* pScene, const DirectX::XMFLOAT3& pos_TopLeftFront, const DirectX::XMFLOAT3& pos_BottomRightBack);

	protected:

	private:
		HRESULT CreateInstanceBuffer(ID3D11Device* pDevice) override;
};