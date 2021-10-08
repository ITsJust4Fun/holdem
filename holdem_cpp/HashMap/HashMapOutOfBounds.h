#pragma once
#include "HashMapException.h"
class HashMapOutOfBounds :
	public HashMapException
{
public:
	HashMapOutOfBounds();
	const char* getError();
	virtual ~HashMapOutOfBounds();
};

