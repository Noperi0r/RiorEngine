#ifndef _APPLICATIONCLASS_H_
#define _APPLICATIONCLASS_H_

//#include <Windows.h>
//#include <windows.h>
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

/// <summary>
/// Main class that is used to render the scene by invoking all needed class objects for the object.
/// </summary>
class ApplicationClass
{
public:
	ApplicationClass();
	ApplicationClass(const ApplicationClass&);
	~ApplicationClass();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* m_Direct3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	ColorShaderClass* m_ColorShader;
};

#endif