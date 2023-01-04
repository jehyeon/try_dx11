#include "Stdafx.h""
#include "CameraClass.h"

CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}

XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::Render()
{
	XMFLOAT3 up, position, lookAt;
	XMVECTOR upVector, positionVector, lookAtVector;

	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// 위쪽을 가리키는 벡터 설정
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// XMVECTOR 구조체 로드
	upVector = XMLoadFloat3(&up);

	// 카메라의 위치 설정
	position = m_position;

	positionVector = XMLoadFloat3(&position);

	// 카메라가 찾고 있는 위치 설정
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	lookAtVector = XMLoadFloat3(&lookAt);

	// yaw (y), pitch (x), roll(z) 축의 회전값을 라디안 단위로 설정
	pitch = m_rotation.x * 0.0174532925f;
	yaw = m_rotation.y * 0.0174532925f;
	roll = m_rotation.z * 0.0174532925f;

	// 회전 행렬 생성
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt과 up 벡터를 회전 행렬로 변형
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// 3개의 벡터로 view 행렬 생성
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}