#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_targaData = 0;
	m_texture = 0;
	m_textureView = 0;
}

TextureClass::TextureClass(const TextureClass&)
{
}

TextureClass::~TextureClass()
{
}

bool TextureClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;
	int height, width;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	// Load the targa image data into memory.
	result = LoadTargaBit(filename); // Get m_targadata as a result
	if (!result) return false;

	// Setup the description of the texture.
	textureDesc.Height = m_height;
	textureDesc.Width = m_width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 32 bit
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT; // Notify DirectX that we are going to load using UpdateSubresource. 
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET; // Flags for binding to pipeline stages
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture.
	hResult = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(hResult)) return false;

	// Set the row pitch of the targa image data.
	rowPitch = (m_width * (m_bpp / 8)) * sizeof(unsigned char);

	// UpdateSubresource is should be used for something that will be loaded once or that gets loaded rarely during loading sequences unlike map/unmap 
	// It puts the data into higher speed memory that gets cache retention preference since it knows you aren't going to remove or reload it anytime soon ??
	
	// The CPU Copies data from the memory to a subresource created in non-mappable memory >> Copy the targa image data into the texture.
	deviceContext->UpdateSubresource(m_texture, 0, NULL, m_targaData, rowPitch, 0);

	// Setup the shader resource view description.
	// Shader resource view: Allows us to have a pointer to set the texture in shaders.
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture.
	hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
	if (FAILED(hResult)) return false;

	// Generate mipmaps for this texture.
	deviceContext->GenerateMips(m_textureView);

	// Release the targa image data now that the image data has been loaded into the texture.
	delete[] m_targaData;
	m_targaData = 0;

	return true;
}

void TextureClass::Shutdown()
{
	if (m_textureView)
	{ 
		m_textureView->Release();
		m_textureView = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	if (m_targaData)
	{
		delete[] m_targaData;
		m_targaData = 0;
	}

	return;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_textureView;
}

int TextureClass::GetWidth()
{
	return m_width;
}

int TextureClass::GetHeight()
{
	return m_height;
}

bool TextureClass::LoadTargaBit(char* filename)
{
	int error, imageSize, index, i, j, k;
	FILE* filePtr;
	unsigned int count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;
	int stepSize;

	// Open the targa file for reading in binary.
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0) return false;

	// Read in the file header.
	count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) return false;

	// Get the important information from the header.
	m_height = (int)targaFileHeader.height;
	m_width = (int)targaFileHeader.width;
	m_bpp = (int)targaFileHeader.bpp;

	if (m_bpp == 32)
	{
		imageSize = m_width * m_height * 4; // 4 bytes per pixel(RGBA)
		k = (m_width * m_height * 4) - (m_width * 4); // Note that targa file is inverse ordered
		stepSize = 4;
	}
	else if (m_bpp == 24)
	{
		imageSize = m_width * m_height * 3; // 3 bytes per pixel(RGB)
		k = (m_width * m_height * 3) - (m_width * 3);
		stepSize = 3;
	}
	else
	{
		return false;
	}

	// Calculate the size of the 32 bit image data.
	//imageSize = m_width * m_height * 4; // 4 bytes per pixel

	targaImage = new unsigned char[imageSize];

	// Read in the targa image data.
	count = (unsigned int)fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) return false;

	error = fclose(filePtr);
	if (error != 0) return false;

	m_targaData = new unsigned char[imageSize]; // Organized data array of targaImage

	index = 0;
	//k = (m_width * m_height * 4) - (m_width * 4); // Note that targa file is inverse ordered
	// Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down and also is not in RGBA order.
	for (j = 0; j < m_height; ++j)
	{
		for (i = 0; i < m_width; ++i)
		{
			m_targaData[index + 0] = targaImage[k + 2]; // Red
			m_targaData[index + 1] = targaImage[k + 1]; // Green
			m_targaData[index + 2] = targaImage[k + 0]; // Blue
			if(m_bpp == 32)
				m_targaData[index + 3] = targaImage[k + 3]; // Alpha
		
			// Increment the indexes into the targa data
			k += stepSize;
			index += stepSize;
		}
		// Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
		k -= (m_width * (stepSize * 2));
	}

	delete[] targaImage;
	targaImage = 0;

	return true;
}
