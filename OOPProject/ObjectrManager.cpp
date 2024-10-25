#include "stdafx.h"

// �ϰ��� update ���� ������� ����
void ObjectrManager::Update(float dt)
{
	for (const auto& object : mObjects)
	{
		object->Update(dt);
	}
	
}
// �ϰ��� draw ���� ������� ����
void ObjectrManager::Draw()
{
	for (auto& object : mObjects)
	{
		object->Draw();
	}
}
// �ϰ��� release ���� ������� ����
void ObjectrManager::Release()
{
	for (auto& object : mObjects)
	{
		object.reset();
	}
}

ObjectrManager::ObjectrManager()
{
}


ObjectrManager::~ObjectrManager()
{
}
