#pragma once
#include <memory>

template <typename T>
class Singleton
{
public:
	void hahaha() {}

	static T* GetInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = std::make_shared<T>();

		return m_pInstance.get();
	}

	static std::shared_ptr<T> GetSharedInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = std::make_shared<T>();

		return m_pInstance;
	}

	static void Free()
	{
		m_pInstance = nullptr;
	}

protected:
	Singleton<T>() {}
	virtual ~Singleton<T>() {}
	
protected:
	static std::shared_ptr<T> m_pInstance;
};

template <typename T>
std::shared_ptr<T> Singleton<T>::m_pInstance = nullptr;