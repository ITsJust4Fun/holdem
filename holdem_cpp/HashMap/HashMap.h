#pragma once

#include <functional>
#include "HashMapIncorrectKey.h"
#include "HashMapOutOfBounds.h"

template<typename T1, typename T2>
class HashMap {
private:
	struct Entry {
		int hash;
		T1 key;
		Entry* next;
		T2 value;
	};
public:
	class iterator {
		friend class HashMap<T1, T2>;
	public:
        T2& operator*() { return curr->value; }
        T1 getKey() { return curr->key; }
		iterator& operator++();
		bool operator!=(const iterator&) const;
		bool operator==(const iterator&) const;
		iterator(const iterator&);
		iterator& operator=(const iterator&);
		~iterator() {}
	private:
		iterator() {}
		HashMap<T1, T2>::Entry* curr;
		HashMap<T1, T2>::Entry* next = nullptr;
		HashMap<T1, T2>* thisHM;
		int index;
		int Nindex;
	};
public:
	HashMap();
	HashMap(const HashMap<T1, T2>&);
	HashMap<T1, T2>& operator=(const HashMap<T1, T2>&);
	void put(const T1& key, const T2& value);
	void putForNullKey(const T2& value);
	T2& operator[](const T1& key);
	iterator begin();
	iterator end();
	void erase(const T1&);
	long long length();
	void clear();
	bool contains(const T1&);
	virtual ~HashMap();
protected:
private:
	iterator* start = nullptr;
	iterator* finish = nullptr;
	Entry** table;
	const int MAXIMUM_CAPACITY = 2147483645;
	double loadFactor;
	int threshold;
	long long size;
	int capacity;
	int hash(const int& h);
	int indexFor(const int& hash, const int& length);
	void addEntry(const int& hash, const T1& key,
				  const T2& value, const int& index);
	void resize(const int& size);
	void transfer(Entry** newTable, const int& len);
};

template<typename T1, typename T2>
HashMap<T1, T2>::iterator::iterator(const iterator& iter) {
	curr = iter.curr;
	next = iter.next;
	index = iter.index;
	Nindex = iter.Nindex;
	thisHM = iter.thisHM;
}

template<typename T1, typename T2>
typename HashMap<T1, T2>::iterator& HashMap<T1, T2>::iterator::operator=(const iterator& iter) {
	curr = iter.curr;
	next = iter.next;
	index = iter.index;
	Nindex = iter.Nindex;
	thisHM = iter.thisHM;
	return *this;
}

template<typename T1, typename T2>
typename HashMap<T1, T2>::iterator& HashMap<T1, T2>::iterator::operator++() {
	if (index >= thisHM->capacity) {
		throw HashMapOutOfBounds();
	}
	curr = next;
	index = Nindex;
	if (next) {
		next = next->next;
	} else {
		curr = thisHM->table[thisHM->capacity];
		next = thisHM->table[thisHM->capacity];
		Nindex = thisHM->capacity;
	}
	if (!next) {
		while (!next && Nindex != thisHM->capacity) {
			Nindex++;
			if (index != thisHM->capacity) {
				next = thisHM->table[Nindex];
			} else {
				next = thisHM->table[thisHM->capacity];
				Nindex = thisHM->capacity;
			}
		}
	}
	return *this;
}

template<typename T1, typename T2>
long long HashMap<T1, T2>::length() {
	return size;
}

template<typename T1, typename T2>
void HashMap<T1, T2>::erase(const T1& key) {
	std::hash<T1> hashFunc;
	int hashCode = hash(hashFunc(key));
	int index = indexFor(hashCode, capacity);
	Entry* ptrE = table[index];
	Entry* prev = nullptr;
	while (ptrE != nullptr) {
		if (ptrE->hash == hashCode && ptrE->key == key) {
			break;
		}
		prev = ptrE;
		ptrE = ptrE->next;
	}
	if (!ptrE) {
		throw HashMapIncorrectKey();
	}
	if (prev) {
		prev->next = ptrE->next;
	} else {
		table[index] = ptrE->next;
	}
	start = nullptr;
	finish = nullptr;
	size--;
	delete ptrE;
}

template<typename T1, typename T2>
bool HashMap<T1, T2>::iterator::operator!=(const iterator& it) const {
	return curr != it.curr;
}

template<typename T1, typename T2>
bool HashMap<T1, T2>::iterator::operator==(const iterator& it) const {
	return curr == it.curr;
}

template<typename T1, typename T2>
typename HashMap<T1, T2>::iterator HashMap<T1, T2>::begin() {
	if (!start) {
		iterator* it = new iterator;
		it->thisHM = this;
		if (size == 0) {
			end();
			start = finish;
			return *start;
		}
		int i = 0;
		Entry* elem = table[i];
		while (!elem) {
			i++;
			elem = table[i];
		}
		it->curr = elem;
		it->index = i;
		it->next = elem->next;
        if (!it->next) {
            while (!it->next && i != (capacity - 1)) {
				i++;
				it->next = table[i];
			}
		}
		it->Nindex = i;
		start = it;
	}
	return *start;
}

template<typename T1, typename T2>
typename HashMap<T1, T2>::iterator HashMap<T1, T2>::end() {
	if (!finish) {
		iterator* it = new iterator;
		it->thisHM = this;
		it->curr = table[capacity];
		it->index = capacity;
		it->Nindex = capacity;
		it->next = nullptr;
		finish = it;
	}
	return *finish;
}

template<typename T1, typename T2>
HashMap<T1, T2>::HashMap() {
	size = 0;
	capacity = 16;
	loadFactor = 0.75;
	threshold = static_cast<int>(capacity * loadFactor);
	table = new Entry*[capacity + 1];
	for (int i = 0; i < capacity; i++) {
		table[i] = nullptr;
	}
	table[capacity] = new Entry;
}

template<typename T1, typename T2>
HashMap<T1, T2>::HashMap(const HashMap<T1, T2>& hm) {
	loadFactor = hm.loadFactor;
	size = hm.size;
	threshold = hm.threshold;
	capacity = hm.capacity;
	table = new Entry*[capacity + 1];
	table[capacity] = new Entry;
	for (int i = 0; i < capacity; i++) {
		Entry* elem = hm.table[i];
		if (elem) {
			Entry* newElem = new Entry;
			table[i] = newElem;
			do {
				newElem->hash = elem->hash;
				newElem->key = elem->key;
				newElem->value = elem->value;
				elem = elem->next;
				if (elem) {
					Entry* tmp = newElem;
					newElem = new Entry;
					tmp->next = newElem;
				} else {
					newElem->next = nullptr;
				}
			} while (elem);
		} else {
			table[i] = elem;
		}
	}
}

template<typename T1, typename T2>
HashMap<T1, T2>& HashMap<T1, T2>::operator=(const HashMap<T1, T2>& hm) {
	delete table[capacity];
	clear();
	loadFactor = hm.loadFactor;
	size = hm.size;
	threshold = hm.threshold;
	capacity = hm.capacity;
	table = new Entry*[capacity + 1];
	table[capacity] = new Entry;
	for (int i = 0; i < capacity; i++) {
		Entry* elem = hm.table[i];
		if (elem) {
			Entry* newElem = new Entry;
			table[i] = newElem;
			do {
				newElem->hash = elem->hash;
				newElem->key = elem->key;
				newElem->value = elem->value;
				elem = elem->next;
				if (elem) {
					Entry* tmp = newElem;
					newElem = new Entry;
					tmp->next = newElem;
				}
				else {
					newElem->next = nullptr;
				}
			} while (elem);
		}
		else {
			table[i] = elem;
		}
	}
	return *this;
}

template<typename T1, typename T2>
void HashMap<T1, T2>::put(const T1& key, const T2& value) {
	std::hash<T1> hashFunc;
	int hashCode = hash(hashFunc(key));
	int index = indexFor(hashCode, capacity);
	Entry* ptrE = table[index];
	while (ptrE != nullptr) {
		if (ptrE->hash == hashCode && ptrE->key == key) {
			ptrE->value = value;
			return;
		}
		ptrE = ptrE->next;
	}
	addEntry(hashCode, key, value, index);
}

template<typename T1, typename T2>
T2& HashMap<T1, T2>::operator[](const T1& key) {
	std::hash<T1> hashFunc;
	int hashCode = hash(hashFunc(key));
	int index = indexFor(hashCode, capacity);
	Entry* elem = table[index];
	while (elem != nullptr) {
		if (elem->hash == hashCode && elem->key == key) {
			return elem->value;
		}
		elem = elem->next;
	}
	throw HashMapIncorrectKey();
}

template<typename T1, typename T2>
void HashMap<T1, T2>::putForNullKey(const T2& value) {
	Entry* ptrE = table[0];
	while (ptrE != nullptr) {
		if (ptrE->key == nullptr) {
			ptrE->value = value;
			return;
		}
		ptrE = ptrE->next;
	}
	addEntry(0, nullptr, value, 0);
}

template<typename T1, typename T2>
int HashMap<T1, T2>::hash(const int& hash) {
	int h = hash;
	h ^= (static_cast<unsigned int>(h) >> 20) ^
		(static_cast<unsigned int>(h) >> 12);
	return h ^ (static_cast<unsigned int>(h) >> 7) ^ 
		(static_cast<unsigned int>(h) >> 4);
}

template<typename T1, typename T2>
int HashMap<T1, T2>::indexFor(const int& hash, const int& length) {
	int h = hash;
	return h & (length - 1);
}

template<typename T1, typename T2>
void HashMap<T1, T2>::addEntry(const int& hash, const T1& key,
							   const T2& value, const int& index) {
	Entry* newElem = new Entry;
	newElem->hash = hash;
	newElem->key = key;
	newElem->value = value;
	if (table[index] == nullptr) {
		newElem->next = nullptr;
	} else {
		newElem->next = table[index];
	}
	table[index] = newElem;
	size++;
	if (threshold <= size) {
		resize(capacity * 2);
	}
	if (start) {
		delete start;
		start = nullptr;
	}
	if (finish) {
		delete finish;
		finish = nullptr;
	}
}

template<typename T1, typename T2>
void HashMap<T1, T2>::resize(const int& size) {
	if (capacity == MAXIMUM_CAPACITY) {
		return;
	}
	int newCapacity = capacity >= 1073741822 ? MAXIMUM_CAPACITY : size;
	Entry** newTable = new Entry*[newCapacity + 1];
	for (int i = 0; i < newCapacity; i++) {
		newTable[i] = nullptr;
	}
	newTable[newCapacity] = new Entry;
	transfer(newTable, newCapacity);
	delete table[capacity];
	delete[] table;
	table = newTable;
	threshold = static_cast<int>(newCapacity * loadFactor);
	capacity = newCapacity;
	if (start) {
		delete start;
		start = nullptr;
	}
	if (finish) {
		delete finish;
		finish = nullptr;
	}
}

template<typename T1, typename T2>
void HashMap<T1, T2>::clear() {
	for (int i = 0; i < capacity; i++) {
		Entry* elem = table[i];
		while (elem != nullptr) {
			Entry* tmp = elem;
			elem = elem->next;
			delete tmp;
		}
		table[i] = nullptr;
	}
	size = 0;
	if (start) {
		delete start;
		start = nullptr;
	}
	if (finish) {
		delete finish;
		finish = nullptr;
	}
}

template<typename T1, typename T2>
bool HashMap<T1, T2>::contains(const T1& key) {
	std::hash<T1> hashFunc;
	int hashCode = hash(hashFunc(key));
	int index = indexFor(hashCode, capacity);
	Entry* elem = table[index];
	while (elem != nullptr) {
		if (elem->hash == hashCode && elem->key == key) {
			return true;
		}
		elem = elem->next;
	}
	return false;
}

template<typename T1, typename T2>
void HashMap<T1, T2>::transfer(Entry** newTable, const int& len) {
	for (int i = 0; i < capacity; i++) {
		Entry* elem = table[i];
		while (elem != nullptr) {
			int index = indexFor(elem->hash, len);
			Entry* tmp = elem->next;
			if (newTable[index] == nullptr) {
				elem->next = nullptr;
			}
			else {
				elem->next = newTable[index];
			}
			newTable[index] = elem;
			elem = tmp;
		}
	}
}

template<typename T1, typename T2>
HashMap<T1, T2>::~HashMap() {
	clear();
	delete table[capacity];
	delete[] table;
}
