#pragma once

#define g_pObjectManager cObjectManager::GetInstance()

class cObject;

class cObjectManager
{
private:
	SINGLETONE(cObjectManager);

private:
	std::set<cObject*> m_setObject;
public:
	void AddObject(cObject* pObject)
	{
		m_setObject.insert(pObject);
	}

	void RemoveObejct(cObject* pObject)
	{
		m_setObject.erase(pObject);
	}

	void Destroy()
	{
		m_setObject.clear();
	}
};

