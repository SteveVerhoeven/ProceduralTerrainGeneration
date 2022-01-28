#pragma once
class Voxel final
{
	public:
		Voxel(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);
		~Voxel() = default;

		Voxel(const Voxel&) = delete;
		Voxel& operator=(const Voxel&) = delete;
		Voxel(Voxel&&) = delete;
		Voxel& operator=(Voxel&&) = delete;

	protected:

	private:
		void CreateFrontFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);
		void CreateLeftFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);
		void CreateRightFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);
		void CreateBottomFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);
		void CreateTopFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size);
		void CreateBackFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier = 0.f);

		void CreateVoxelIndices(std::vector<uint32_t>& indices, const std::vector<Vertex_Input>& vertices);
};
