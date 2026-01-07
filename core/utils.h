#pragma once

#include <assert.h>
#include <string.h>

constexpr int DEFAULT_ARR_CAPACITY = 16;
constexpr int SCALE_MUL = 2;

char* copy_string(const char* str);

int clamp(int value, int min, int max);
float clamp(float value, float min, float max);

template <typename T>
constexpr bool is_str() { return false; }

template <>
constexpr bool is_str<const char*>() { return true; }

template <>
constexpr bool is_str<char*>() { return true; }

template <typename T>
class Array {
private:
	T* array;
	int arrCount;
public:
	Array(int count);
	Array(const Array&) = delete;
	Array& operator=(const Array&) = delete;
	~Array();
	T& get(int index);
	T& operator[](int index);
	int count() const;
};

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
	void remove(T& item);
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

/*template <typename T>
struct KVPair {
	const char* key;
	T value;
};*/

template <typename K, typename V>
struct KVPair {
	K key;
	V value;
};

template <typename K, typename V>
class Map {
protected:
	ArrayList<KVPair<K,V>> array;
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	bool compareKeys(const K& key1, const K& key2) const;
public:
	Map(int size = DEFAULT_ARR_CAPACITY);
	~Map();

	V& get(const K key);
	V& getByIndex(int index);
	bool containsKey(const K key) const;
	void put(const K key, const V& value);
	void remove(const K key);
	void clear();
	const V& operator[](const K key) const;
	V& operator[](const K key);
	KVPair<K, V>* begin();
	KVPair<K, V>* end();
	const KVPair<K, V>* begin() const;
	const KVPair<K, V>* end() const;
};

template<typename T>
inline Array<T>::Array(int count) : arrCount(count) {
	array = new T[arrCount];
}

template<typename T>
inline Array<T>::~Array() {
	delete[] array;
	array = nullptr;
}

template<typename T>
inline T& Array<T>::get(int index) {
	assert(index >= 0 && index < arrCount && "Index out of bounds");

	return array[index];
}

template <typename T>
inline T& Array<T>::operator[](int index) {
	return get(index);
}

template<typename T>
inline int Array<T>::count() const
{
	return arrCount;
}

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
inline void ArrayList<T>::remove(T& item) {
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
	bool swapped = false;

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

template<typename K, typename V>
inline bool Map<K, V>::compareKeys(const K& key1, const K& key2) const
{
	if constexpr (is_str<K>()) {
		return strcmp(key1, key2) == 0;
	}
	else {
		return key1 == key2;
	}
}

template<typename K, typename V>
inline Map<K,V>::Map(int size) : array(size) {}

template<typename K, typename V>
inline Map<K, V>::~Map() {
	clear();
}

template<typename K, typename V>
inline V& Map<K, V>::get(K key) {
	if (key == NULL) {
		assert(false && "Key must not be null");
		return array[0].value;
	}

	for (KVPair<K,V>& item : array) {
		if (compareKeys(key, item.key))
			return item.value;
	}

	assert(false && "Key does not exists");
	return array[0].value;
}

template<typename K, typename V>
inline V& Map<K, V>::getByIndex(int index)
{
	return array.get(index).value;
}

template<typename K, typename V>
inline bool Map<K, V>::containsKey(const K key) const
{
	for (const KVPair<K, V>& item : array) {
		if (compareKeys(key, item.key))
			return true;
	}

	return false;
}

template<typename K, typename V>
inline void Map<K, V>::put(const K key, const V& value) {
	for (KVPair<K, V>& item : array) {
		if (compareKeys(key, item.key)) {
			item.value = value;
			return;
		}
	}

	if constexpr (is_str<K>()) {
		array.add({ copy_string(key), value });
	} else {
		array.add({ key, value });
	}
}

template<typename K, typename V>
inline void Map<K, V>::remove(const K key) {
	for (int i = 0; i < array.count(); i++) {
		if (compareKeys(key, array[i].key)) {
			if constexpr (is_str<K>()) {
				delete[] array[i].key;
				array[i].key = nullptr;
			}
			array.removeAt(i);
			return;
		}
	}
}

template<typename K, typename V>
inline void Map<K, V>::clear() {
	if constexpr (is_str<K>()) {
		for (KVPair<K, V>& item : array) {
			delete[] item.key;
			item.key = nullptr;
		}
	}
	array.clear();
}

template<typename K, typename V>
inline const V& Map<K, V>::operator[](const K key) const {
	for (const KVPair<K, V>& item : array) {
		if (compareKeys(item.key, key)) {
			return item.value;
		}
	}

	assert(false && "Key does not exists");
}

template<typename K, typename V>
inline V& Map<K, V>::operator[](const K key) {
	for (KVPair<K, V>& item : array) {
		if (compareKeys(item.key, key)) {
			return item.value;
		}
	}

	if constexpr (is_str<K>())
		array.add({ copy_string(key), V() });
	else
		array.add({ key, V()});

	return array[array.count() - 1].value;
}

template<typename K, typename V>
inline KVPair<K, V>* Map<K, V>::begin()
{
	return array.begin();
}

template<typename K, typename V>
inline KVPair<K, V>* Map<K, V>::end()
{
	return array.end();
}

template<typename K, typename V>
inline const KVPair<K, V>* Map<K, V>::begin() const
{
	return array.begin();
}

template<typename K, typename V>
inline const KVPair<K, V>* Map<K, V>::end() const
{
	return array.end();
}