#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;

#include "TextureClass.h"


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
		XMFLOAT3 normal;
	};

	struct ModelType 
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* modelFilename, char* textureFilename);
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

	bool LoadCustomModel(char* modelFilename);
	bool LoadObjModel(char* modelFilename);
	void ReleaseModel();

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer; // DX11 Buffers generally use the genereic ID3D11Buffer Type, and more clearly identified by a buffer description when first created.
	int m_vertexCount, m_indexCount; // Keep track of the size of each buffer
	TextureClass* m_texture;
	ModelType* m_model; // Array. It will be used to read in and hold the model data before it is placed in the vertex buffer.
};

#endif 