#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <stdio.h>

class TextureClass
{
private: 
	struct TargaHeader // Define this structure to make reading in the targa file data easier
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	int GetWidth();
	int GetHeight();

private:
	bool LoadTargaBit(char* filename); // Targa reading function

private:
	unsigned char* m_targaData; // Hold the raw Targa data read stright in from the file
	ID3D11Texture2D* m_texture; // Strucured texture data that DirectX will use for rendering
	ID3D11ShaderResourceView* m_textureView; // Shader uses it to access the texture data when drawing
	int m_width, m_height; // Dimensions of the texture
	int m_bpp;
};

#endif 