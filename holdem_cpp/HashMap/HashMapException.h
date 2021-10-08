#pragma once
class HashMapException {
public:
	HashMapException();
	virtual const char* getError() = 0;
	virtual ~HashMapException();
private:
};

