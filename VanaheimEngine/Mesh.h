#pragma once

class Material;
class Mesh final
{
	public:
		Mesh();
		Mesh(const std::string& path);
		~Mesh();

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) noexcept = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) noexcept = delete;
		
		void Initialize();
		void Initialize(const std::vector<Vertex_Input>& vertices, const std::vector<uint32_t>& indices);
		void ReInitialize(const std::vector<Vertex_Input>& vertices, const std::vector<uint32_t>& indices);
		void PostInitialize(Material* pMaterial);

		void IncrementInstanceCount() { ++m_AmountInstances; }
		void DecrementInstanceCount() { ++m_AmountInstances; }
		
		ID3D11Buffer* GetIBuffer() const { return m_pIBuffer; }
		ID3D11Buffer* GetVBuffer() const { return m_pVBuffer; }
		ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }
		
		uint32_t GetAmountIndices() const { return m_AmountIndices; }
		uint32_t GetAmountVertices() const { return m_AmountVertices; }
		uint32_t GetAmountInstances() const { return m_AmountInstances; }

		void SetIsRendered(const bool isRendered) { m_IsRendered = isRendered; }
		bool GetIsRendered() const { return m_IsRendered; }

	private:
		bool m_IsInitialized;
		bool m_IsRendered;

		std::string m_FilePath;

		uint32_t m_AmountIndices;
		uint32_t m_AmountVertices;
		uint32_t m_AmountInstances;

		ID3D11Buffer* m_pIBuffer;
		ID3D11Buffer* m_pVBuffer;
		ID3D11InputLayout* m_pInputLayout;

		HRESULT CreateVertex_InputLayout(ID3D11Device* pDevice, Material* pMaterial);
		HRESULT CreateIndexBuffer(ID3D11Device* pDevice, const std::vector<uint32_t>& iBuffer);
		HRESULT CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<Vertex_Input>& vBuffer);
};