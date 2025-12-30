#pragma once

#include <assert.h>
#include <string.h>

constexpr int DEFAULT_ARR_CAPACITY = 16;
constexpr int SCALE_MUL = 2;

char* copy_string(const char* str);

int clamp(int value, int min, int max);
float clamp(float value, float min, float max);

template <typename T>
class ArrayList {
private:
	T* array;
	int arrCount;
	int arrCapacity;
	void resize(int newCapacity);
	ArrayList(const ArrayList&) = delete;
	ArrayList& operator=(const ArrayList&) = delete;
public:
	ArrayList(int capacity = DEFAULT_ARR_CAPACITY);
	~ArrayList();
	T& get(int index);
	void add(const T& item);
	void insert(int index, const T& item);
	void removeAt(int index);
	void remove(const T& item);
	void removeIf(bool (*cond)(const T&), void (*onRemove)(T&) = nullptr);
	int count() const;
	void clear();
	void sort(bool (*compare)(const T&, const T&));
	T& operator[](int index);
	T* begin();
	T* end();
	const T* begin() const;
	const T* end() const;
};

template <typename T>
struct KVPair {
	const char* key;
	T value;
};

template <typename T>
class Map {
private:
	ArrayList<KVPair<T>> array;
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
public:
	Map(int size = DEFAULT_ARR_CAPACITY);
	~Map();

	T& get(const char* key);
	T& getByIndex(int index);
	bool containsKey(const char* key) const;
	void put(const char* key, const T& value);
	void remove(const char* key);
	void clear();
	const T& operator[](const char* key) const;
	T& operator[](const char* key);
	KVPair<T>* begin();
	KVPair<T>* end();
	const KVPair<T>* begin() const;
	const KVPair<T>* end() const;
};

template <class T>
class Singleton {
protected:
	Singleton() = default;
	~Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
public:
	static T& getInstance();
};

template<typename T>
inline void ArrayList<T>::resize(int newCapacity) {
	if (newCapacity < 2) newCapacity = 2;

	int toCopy = (arrCount < newCapacity) ? arrCount : newCapacity;
	T* newArray = new T[newCapacity];

	for (int i = 0; i < toCopy; i++) {
		newArray[i] = array[i];
	}

	delete[] array;
	array = newArray;
	arrCapacity = newCapacity;
	arrCount = toCopy;
}

template<typename T>
inline ArrayList<T>::ArrayList(int capacity) : arrCapacity(capacity), arrCount(0) {
	array = new T[capacity];
}

template<typename T>
inline ArrayList<T>::~ArrayList() {
	delete[] array;
	array = nullptr;
}

template<typename T>
inline T& ArrayList<T>::get(int index) {
	assert(index >= 0 && index < arrCount && "Index out of bounds");

	return array[index];
}

template<typename T>
inline void ArrayList<T>::add(const T& item) {
	if (arrCount >= arrCapacity)
		resize(arrCapacity * SCALE_MUL);

	array[arrCount] = item;
	arrCount++;
}

template<typename T>
inline void ArrayList<T>::insert(int index, const T& item) {
	assert(index >= 0 && index <= arrCount && "Index out of bounds");

	if (arrCount >= arrCapacity)
		resize(arrCapacity * SCALE_MUL);

	for (int i = arrCount; i > index; i--) {
		array[i] = array[i - 1];
	}

	arrCount++;
	array[index] = item;
}

template<typename T>
inline void ArrayList<T>::removeAt(int index) {
	assert(index >= 0 && index < arrCount && "Index out of bounds");

	for (int i = index; i < arrCount - 1; i++) {
		array[i] = array[i + 1];
	}

	arrCount--;

	if (arrCount > DEFAULT_ARR_CAPACITY &&
		arrCount < arrCapacity / (SCALE_MUL * 2)
		) {
		resize(arrCapacity / SCALE_MUL);
	}
}

template<typename T>
inline void ArrayList<T>::remove(const T& item) {
	for (int i = 0; i < arrCount; i++) {
		if (array[i] == item) {
			removeAt(i);
			break;
		}
	}
}

template<typename T>
inline void ArrayList<T>::removeIf(
	bool (*cond)(const T&), 
	void (*onRemove)(T&)
) {
	int i = 0, j = 0;

	for (; i < arrCount; i++) {
		if (!cond(array[i])) {
			array[j++] = array[i];
		}
		else {
			if (onRemove != nullptr)
				onRemove(array[i]);
		}
	}

	arrCount = j;
}

template<typename T>
inline int ArrayList<T>::count() const {
	return arrCount;
}

template<typename T>
inline void ArrayList<T>::clear() {
	arrCount = 0;
	resize(DEFAULT_ARR_CAPACITY);
}

// Bubble sort z flag¹ swapped
template<typename T>
inline void ArrayList<T>::sort(bool(*compare)(const T&, const T&)) {
	bool swapped;

	for (int i = 0; i < arrCount - 1; i++) {
		for (int j = 0; j < arrCount - i - 1; j++) {
			swapped = false;

			if (compare(array[j], array[j + 1])) {
				T temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;

				swapped = true;
			}
		}
		if (!swapped) break;
	}
}

template<typename T>
inline T& ArrayList<T>::operator[](int index) {
	return get(index);
}

template<typename T>
inline T* ArrayList<T>::begin()
{
	return array;
}

template<typename T>
inline T* ArrayList<T>::end()
{
	return array + arrCount;
}

template<typename T>
inline const T* ArrayList<T>::begin() const {
	return array;
}

template<typename T>
inline const T* ArrayList<T>::end() const {
	return array + arrCount;
}

template<typename T>
inline Map<T>::Map(int size) : array(size) {}

template<typename T>
inline Map<T>::~Map() {
	clear();
}

template<typename T>
inline T& Map<T>::get(const char* key) {
	static T dummy;
	for (KVPair<T>& item : array) {
		if (strcmp(key, item.key) == 0) {
			return item.value;
		}
	}

	assert(false && "Key does not exists");
	return dummy;
}

template<typename T>
inline T& Map<T>::getByIndex(int index)
{
	return array.get(index).value;
}

template<typename T>
inline bool Map<T>::containsKey(const char* key) const
{
	for (const KVPair<T>& item : array) {
		if (strcmp(key, item.key) == 0) {
			return true;
		}
	}

	return false;
}

template<typename T>
inline void Map<T>::put(const char* key, const T& value) {
	for (KVPair<T>& item : array) {
		if (strcmp(key, item.key) == 0) {
			item.value = value;
			return;
		}
	}

	array.add({ copy_string(key), value });
}

template<typename T>
inline void Map<T>::remove(const char* key) {
	for (int i = 0; i < array.count(); i++) {
		if (strcmp(array[i].key, key) == 0) {
			delete[] array[i].key;
			array[i].key = nullptr;
			array.removeAt(i);
			return;
		}
	}
}

template<typename T>
inline void Map<T>::clear() {
	for (KVPair<T>& item : array) {
		delete[] item.key;
		item.key = nullptr;
	}
	array.clear();
}

template<typename T>
inline const T& Map<T>::operator[](const char* key) const {
	for (const KVPair<T>& item : array) {
		if (strcmp(key, item.key) == 0) {
			return item.value;
		}
	}

	assert(false && "Key does not exists");
}

template<typename T>
inline T& Map<T>::operator[](const char* key) {
	for (KVPair<T>& item : array) {
		if (strcmp(key, item.key) == 0) {
			return item.value;
		}
	}

	array.add({ copy_string(key), T() });

	return array[array.count() - 1].value;
}

template<typename T>
inline KVPair<T>* Map<T>::begin()
{
	return array.begin();
}

template<typename T>
inline KVPair<T>* Map<T>::end()
{
	return array.end();
}

template<typename T>
inline const KVPair<T>* Map<T>::begin() const
{
	return array.begin();
}

template<typename T>
inline const KVPair<T>* Map<T>::end() const
{
	return array.end();
}

template<class T>
inline T& Singleton<T>::getInstance() {
	static T instance;

	return instance;
}
