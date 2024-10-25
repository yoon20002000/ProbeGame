#include "stdafx.h"

// 일괄적 update 현재 사용하지 않음
void ObjectrManager::Update(float dt)
{
	for (const auto& object : mObjects)
	{
		object->Update(dt);
	}
	
}
// 일괄적 draw 현재 사용하지 않음
void ObjectrManager::Draw()
{
	for (auto& object : mObjects)
	{
		object->Draw();
	}
}
// 일괄적 release 현재 사용하지 않음
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
