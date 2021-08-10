#pragma once
#include <iostream>

template< typename T>
class UxSingleTon
{
public:
	UxSingleTon();
	virtual ~UxSingleTon();

private:
	static T* s_instance;

public:
	static T& GetInstance();
};

template< typename T >
T* UxSingleTon< T >::s_instance = nullptr;

template< typename T >
UxSingleTon< T >::UxSingleTon()
{
	if ( s_instance )
		std::cout << "Not make First" << std::endl;

	s_instance = (T*)( this );
}

template< typename T >
UxSingleTon< T >::~UxSingleTon()
{

}

template< typename T >
T& UxSingleTon< T >::GetInstance()
{
	return *s_instance;
}