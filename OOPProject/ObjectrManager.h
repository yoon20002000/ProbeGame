#pragma once
// 현재 object 생성 및 return 용으로 사용
class ObjectrManager final
{
private:
	// 모든 object를 저장하고 관리할 list
	list<unique_ptr<GameObject>> mObjects;
public:
	// singleton을 위한 static 함수
	static ObjectrManager& Call()
	{
		static ObjectrManager instance;
		return instance;
	}
public :
	void Update(float dt);
	void Draw();
	void Release();

public :
	// object tag를 이용하여 원하는 object를 반환하는 함수
	GameObject* GetOject(string objTag)
	{
		GameObject* p;
		for (auto& object : mObjects)
		{
			if(object->GetTag() == objTag)
			{
				p = object.get();
			}
		}
		return p;
	}
// ellipses variadic
// template class
// mapping 되는 생성자가 없을경우 에러를 발생하나 에러 코드를 제대로 알려주지 않아 디버깅이 힘들수 있음
public:
template<class T, class... Types> inline
T* Create(Types&&... args)
{
	// 모든 파라미터를 받아와 해당하는 class로 생성하여 objectlist 에 push
	mObjects.push_back(unique_ptr<T>(new T(forward<Types>(args)...)));
	return dynamic_cast<T*>(mObjects.back().get());
}

public:
	ObjectrManager();
	~ObjectrManager();
};

