#pragma once
#include <vector>
#include <string>

struct Face
{
	/*
	DirectX::XMFLOAT3
		-> x = v
		-> y = vt
		-> z = vn
	*/
	Face(const DirectX::XMINT3& vertex0,
		 const DirectX::XMINT3& vertex1,
		 const DirectX::XMINT3& vertex2)
	{
		v0 = vertex0;
		v1 = vertex1;
		v2 = vertex2;
	}

	DirectX::XMINT3 v0;
	DirectX::XMINT3 v1;
	DirectX::XMINT3 v2;
};

class Mesh;
class OBJWriter final
{
	public:
		OBJWriter() = default;
		~OBJWriter() = default;

		OBJWriter(const OBJWriter&) = delete;
		OBJWriter(OBJWriter&&) noexcept = delete;
		OBJWriter& operator=(const OBJWriter&) = delete;
		OBJWriter& operator=(OBJWriter&&) noexcept = delete;

		std::string WriteOBJ(const std::string& fileName, const std::string& filePath, const std::vector<Vertex_Input>& vBuffer, const std::vector<uint32_t>& iBuffer) const;

	private:

};