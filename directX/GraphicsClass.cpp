#include "Stdafx.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{

}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Direct3D 객체 생성
	m_Direct3D = new D3DClass;

	if (!m_Direct3D)
	{
		return false;
	}

	// Direct3D 객체 초기화
	if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// m_Camera 객체 생성
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// m_Model 객체 생성
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	if (!m_Model->Initialize(m_Direct3D->GetDevice()))
	{
		MessageBox(hwnd, L"Could not initilize the model object.", L"Error", MB_OK);
		return false;
	}

	// m_ColorShader 객체 생성
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = 0;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = 0;
	}
}

bool GraphicsClass::Frame()
{
	return Render();
}

bool GraphicsClass::Render()
{
	// 씬을 그리기 위해 버퍼를 지움
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라에 위치에 따라 뷰 행렬 생성
	m_Camera->Render();

	// 카메라 및 d3d 객체에서 world, view, projection 행렬을 가져옴
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// model vertex와 index buffer를 파이프라인에 배치하여 렌더링 준비
	m_Model->Render(m_Direct3D->GetDeviceContext());

	if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
	{
		// color shader를 사용하여 모델 렌더링
		return false;
	}

	// 버퍼 내용을 화면에 출력
	m_Direct3D->EndScene();

	return true;
}