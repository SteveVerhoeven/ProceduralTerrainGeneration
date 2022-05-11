#pragma once
#include <vector>
#include <string>

class Material;
struct Vertex_Input;
class Mesh_Base
{
	public:
		Mesh_Base();
		Mesh_Base(const std::string& path);
		virtual ~Mesh_Base();

		Mesh_Base(const Mesh_Base&) = delete;
		Mesh_Base(Mesh_Base&&) noexcept = delete;
		Mesh_Base& operator=(const Mesh_Base&) = delete;
		Mesh_Base& operator=(Mesh_Base&&) noexcept = delete;

		void Initialize();
		void Initialize(const std::vector<Vertex_Input>& vertices, const std::vector<uint32_t>& indices);
		virtual void PostInitialize(Material* pMaterial);

		ID3D11Buffer* GetIBuffer() const { return m_pIBuffer; }
		ID3D11Buffer* GetVBuffer() const { return m_pVBuffer; }
		ID3D11Buffer* GetInstanceBuffer() const { return m_pInstanceBuffer; }
		ID3D11InputLayout* GetInputLayout() const { return m_pInputLayout; }

		uint32_t GetAmountIndices() const { return m_AmountIndices; }
		uint32_t GetAmountVertices() const { return m_AmountVertices; }
		uint32_t GetAmountInstances() const { return m_AmountInstances; }

		void SetIsRendered(const bool isRendered) { m_IsRendered = isRendered; }
		bool GetIsRendered() const { return m_IsRendered; }

		void IncrementInstanceCount() { ++m_AmountInstances; }
		void DecrementInstanceCount() { --m_AmountInstances; }

		/** Serializing */
		const bool GetIsInitialized() const { return m_IsInitialized; }
		const std::string& GetFilePath() const { return m_FilePath; }

	protected:
		bool m_IsInitialized;
		bool m_IsRendered;

		std::string m_FilePath;

		uint32_t m_AmountIndices;
		uint32_t m_AmountVertices;
		uint32_t m_AmountInstances;

		ID3D11Buffer* m_pIBuffer;
		ID3D11Buffer* m_pVBuffer;
		ID3D11Buffer* m_pInstanceBuffer;
		ID3D11InputLayout* m_pInputLayout;

		std::vector<DirectX::XMFLOAT3> m_InstancePositionData;

		HRESULT CreateVertex_InputLayout(ID3D11Device* pDevice, Material* pMaterial);
		HRESULT CreateIndexBuffer(ID3D11Device* pDevice, const std::vector<uint32_t>& iBuffer);
		HRESULT CreateVertexBuffer(ID3D11Device* pDevice, const std::vector<Vertex_Input>& vBuffer);
		
		virtual HRESULT CreateInstanceBuffer(ID3D11Device* pDevice) = 0;

	private:

};