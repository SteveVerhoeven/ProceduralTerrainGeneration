#pragma once
#include "Mesh_Base.h"
#include <vector>

enum class LineType
{
	UNKNOWN,
	VERTICAL,
	HORIZONTAL_X,
	HORIZONTAL_Z
};
enum class LineCorner
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

class Line final : public Mesh_Base
{
	public:
		Line(const DirectX::XMFLOAT3& position_Start, const DirectX::XMFLOAT3& position_End, const LineType& LineType, const float length, const float height);
		Line(const LineCorner& sc1, const LineCorner& sc2, const LineType& LineType, const float sizeXZ, const float sizeY);
		~Line() = default;

		Line(const Line&) = delete;
		Line(Line&&) noexcept = delete;
		Line& operator=(const Line&) = delete;
		Line& operator=(Line&&) noexcept = delete;

		virtual void PostInitialize(Material* pMaterial) override;

		std::vector<DirectX::XMFLOAT4> GetLength() const 
		{ return { { m_InstanceLengthDataV1[0] }, { m_InstanceLengthDataV2[0] } }; }
		LineType GetLineType() const { return m_LineType; }

		ID3D11Buffer* GetInstanceBufferLengthV1() const { return m_pInstanceBuffer_Length_V1; }
		ID3D11Buffer* GetInstanceBufferLengthV2() const { return m_pInstanceBuffer_Length_V2; }

		void StoreLengthsFromInstances(const std::vector<DirectX::XMFLOAT4>& lenghts);
		void StorePositionsFromInstances(const DirectX::XMFLOAT3& positions);

		bool GetRenderLine() const { return m_RenderLine; }

	private:
		bool m_RenderLine;
		LineType m_LineType;
		ID3D11Buffer* m_pInstanceBuffer_Length_V1;
		ID3D11Buffer* m_pInstanceBuffer_Length_V2;
		std::vector<DirectX::XMFLOAT4> m_InstanceLengthDataV1;
		std::vector<DirectX::XMFLOAT4> m_InstanceLengthDataV2;

		HRESULT CreateInstanceBuffer(ID3D11Device* pDevice) override;
		HRESULT CreateInstanceBufferLengthV1(ID3D11Device* pDevice);
		HRESULT CreateInstanceBufferLengthV2(ID3D11Device* pDevice);

		DirectX::XMFLOAT3 GetCornerPosition(const LineCorner& LineCorner, const float sizeXZ, const float sizeY, const LineType& LineType) const;
};