#include "Stdafx.h"
#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass& other)
{
}

InputClass::~InputClass()
{

}

void InputClass::Initialize()
{
	// 키 배열 초기화
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int input)
{
	m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
	m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	// 현재 키 값이 눌려있는지 상태 반환
	return m_keys[key];
}