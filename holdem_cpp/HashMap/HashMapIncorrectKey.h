#pragma once
#include "HashMapException.h"
class HashMapIncorrectKey : public HashMapException
{
public:
	HashMapIncorrectKey();
	virtual const char* getError();
	virtual ~HashMapIncorrectKey();
private:
};

