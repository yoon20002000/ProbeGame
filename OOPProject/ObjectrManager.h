#pragma once
// ���� object ���� �� return ������ ���
class ObjectrManager final
{
private:
	// ��� object�� �����ϰ� ������ list
	list<unique_ptr<GameObject>> mObjects;
public:
	// singleton�� ���� static �Լ�
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
	// object tag�� �̿��Ͽ� ���ϴ� object�� ��ȯ�ϴ� �Լ�
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
// mapping �Ǵ� �����ڰ� ������� ������ �߻��ϳ� ���� �ڵ带 ����� �˷����� �ʾ� ������� ����� ����
public:
template<class T, class... Types> inline
T* Create(Types&&... args)
{
	// ��� �Ķ���͸� �޾ƿ� �ش��ϴ� class�� �����Ͽ� objectlist �� push
	mObjects.push_back(unique_ptr<T>(new T(forward<Types>(args)...)));
	return dynamic_cast<T*>(mObjects.back().get());
}

public:
	ObjectrManager();
	~ObjectrManager();
};

