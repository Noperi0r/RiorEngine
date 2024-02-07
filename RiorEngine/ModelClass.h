#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


// Includes
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// Responsible for encapsulating the geometry for 3D models
/// </summary>
class ModelClass 
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer; // DX11 Buffers generally use the genereic ID3D11Buffer Type, and more clearly identified by a buffer description when first created.
	int m_vertexCount, m_indexCount; // Keep track of the size of each buffer

};

#endif 