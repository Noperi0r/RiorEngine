#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>

#include "TextureClass.h"

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
		XMFLOAT2 texture;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext); 

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void ReleaseTexture();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer; // DX11 Buffers generally use the genereic ID3D11Buffer Type, and more clearly identified by a buffer description when first created.
	int m_vertexCount, m_indexCount; // Keep track of the size of each buffer
	TextureClass* m_texture;
};

#endif 