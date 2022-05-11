#include "VanaheimPCH.h"
#include "Voxel.h"

Voxel::Voxel(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	// Side Front
	CreateFrontFace(vertices, indices, position, size, bottomModifier);
	// Side Left
	CreateLeftFace(vertices, indices, position, size, bottomModifier);
	// Side Right
	CreateRightFace(vertices, indices, position, size, bottomModifier);
	// Side Bottom
	CreateBottomFace(vertices, indices, position, size, bottomModifier);
	// Side Top
	CreateTopFace(vertices, indices, position, size);
	// Side Back
	CreateBackFace(vertices, indices, position, size, bottomModifier);
}

void Voxel::CreateFrontFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z - modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z - modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}
void Voxel::CreateLeftFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z + modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z - modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}
void Voxel::CreateRightFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z - modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z + modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}
void Voxel::CreateBottomFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z - modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}
void Voxel::CreateTopFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z + modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z - modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z + modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z - modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}
void Voxel::CreateBackFace(std::vector<Vertex_Input>& vertices, std::vector<uint32_t>& indices, const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT2& size, const float bottomModifier)
{
	const float modifier{ 0.5f };

	Vertex_Input vLT{};		Vertex_Input vRT{};
	Vertex_Input vLB{};		Vertex_Input vRB{};

	vLT.Position = DirectX::XMFLOAT3(position.x + modifier, position.y + modifier, position.z + modifier);
	vLB.Position = DirectX::XMFLOAT3(position.x + modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);
	vRT.Position = DirectX::XMFLOAT3(position.x - modifier, position.y + modifier, position.z + modifier);
	vRB.Position = DirectX::XMFLOAT3(position.x - modifier, position.y - ((bottomModifier == 0.f) ? modifier : bottomModifier), position.z + modifier);

	vLT.UV = vLB.UV = vRT.UV = vRB.UV = DirectX::XMFLOAT2((position.x / float(size.x)), (-position.z / float(size.y)));

	vertices.push_back(vLT);
	vertices.push_back(vLB);
	vertices.push_back(vRT);
	vertices.push_back(vRB);

	CreateVoxelIndices(indices, vertices);
}

void Voxel::CreateVoxelIndices(std::vector<uint32_t>& indices, const std::vector<Vertex_Input>& vertices)
{
	// i		 i + 1
	// +---------+
	// |         |
	// |		 |
	// |		 |
	// +---------+
	// i + w	 i + w + 1

	indices.push_back(vertices.size() - 4);
	indices.push_back(vertices.size() - 1);
	indices.push_back(vertices.size() - 3);

	indices.push_back(vertices.size() - 1);
	indices.push_back(vertices.size() - 4);
	indices.push_back(vertices.size() - 2);
}