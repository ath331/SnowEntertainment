#pragma once
#include <iostream>

template< typename T>
class SingleTonClass
{
public:
	SingleTonClass();
	virtual ~SingleTonClass();

private:
	static T* _instance;

public:
	static T& GetInstance();
};

template< typename T >
T* SingleTonClass< T >::_instance = nullptr;

template< typename T >
SingleTonClass< T >::SingleTonClass()
{
	if ( _instance )
		std::cout << "Not make First" << std::endl;

	_instance = (T*)( this );
}

template< typename T >
SingleTonClass< T >::~SingleTonClass()
{

}

template< typename T >
T& SingleTonClass< T >::GetInstance()
{
	return *_instance;
}