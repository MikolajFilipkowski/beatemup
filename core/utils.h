#pragma once

#include <assert.h>
#include <string.h>

constexpr int DEFAULT_ARR_CAPACITY = 16;
constexpr int SCALE_MUL = 2;

char* copy_string(const char* a_Str);

int clamp(int a_Val, int a_Min, int a_Max);
float clamp(float a_Val, float a_Min, float a_Max);

template <typename T>
constexpr bool isStr() { return false; }

template <>
constexpr bool isStr<const char*>() { return true; }

template <>
constexpr bool isStr<char*>() { return true; }

template <typename T>
class Array {
private:
	T* m_Array;
	int m_ArrCount;
public:
	Array(int a_Count);
	Array(const Array&) = delete;
	Array& operator=(const Array&) = delete;
	~Array();
	T& get(int a_Index);
	T& operator[](int a_Index);
	int count() const;
	T* begin();
	T* end();
};

template <typename T>
class ArrayList {
private:
	T* m_Array;
	int m_ArrCount{ 0 };
	int m_ArrCapacity;
	void resize(int a_Capacity);
	ArrayList(const ArrayList&) = delete;
	ArrayList& operator=(const ArrayList&) = delete;
public:
	ArrayList(int a_Capacity = DEFAULT_ARR_CAPACITY);
	~ArrayList();
	T& get(int a_Index);
	void add(const T& a_Item);
	void insert(int a_Index, const T& a_Item);
	void removeAt(int a_Index);
	void remove(T& a_Item);
	void removeIf(bool (*a_Condition)(const T&), void (*a_OnRemove)(T&) = nullptr);
	int count() const;
	void clear();
	void sort(bool (*a_Compare)(const T&, const T&));
	T& operator[](int a_Index);
	T* begin();
	T* end();
	const T* begin() const;
	const T* end() const;
};

template <typename K, typename V>
struct KVPair {
	K key;
	V value;
};

template <typename K, typename V>
class Map {
protected:
	ArrayList<KVPair<K,V>> m_Array;
	Map(const Map&) = delete;
	Map& operator=(const Map&) = delete;
	bool compareKeys(const K& a_Key1, const K& a_Key2) const;
public:
	Map(int size = DEFAULT_ARR_CAPACITY);
	~Map();

	V& get(const K a_Key);
	V& getByIndex(int a_Index);
	bool containsKey(const K a_Key) const;
	void put(const K a_Key, const V& a_Val);
	void remove(const K a_Key);
	void clear();
	int count() const;;
	const V& operator[](const K a_Key) const;
	V& operator[](const K a_Key);
	KVPair<K, V>* begin();
	KVPair<K, V>* end();
	const KVPair<K, V>* begin() const;
	const KVPair<K, V>* end() const;
};

template <typename T>
class CircularQueue {
protected:
	T* m_Array;
	int m_Size;
	int m_ItemCount;
	int m_Read;
	int m_Write;
public:
	CircularQueue(int a_Size);
	virtual ~CircularQueue();
	void push(T a_Val);
	T pop();
	T& peek();
	T& peekAt(int a_idxFromRead);
	void clear();
	int count() const;
	int maxSize() const;
};

template<typename T>
inline Array<T>::Array(int count) : m_ArrCount(count) {
	m_Array = new T[m_ArrCount];
}

template<typename T>
inline Array<T>::~Array() {
	delete[] m_Array;
	m_Array = nullptr;
}

template<typename T>
inline T& Array<T>::get(int index) {
	assert(index >= 0 && index < m_ArrCount && "Index out of bounds");

	return m_Array[index];
}

template <typename T>
inline T& Array<T>::operator[](int index) {
	return get(index);
}

template<typename T>
inline int Array<T>::count() const
{
	return m_ArrCount;
}

template<typename T>
inline T* Array<T>::begin()
{
	return m_Array;
}

template<typename T>
inline T* Array<T>::end()
{
	return m_Array + m_ArrCount;
}

template<typename T>
inline void ArrayList<T>::resize(int a_Capacity) {
	if (a_Capacity < 2) a_Capacity = 2;

	int toCopy = (m_ArrCount < a_Capacity) ? m_ArrCount : a_Capacity;
	T* newArray = new T[a_Capacity];

	for (int i = 0; i < toCopy; i++) {
		newArray[i] = m_Array[i];
	}

	delete[] m_Array;
	m_Array = newArray;
	m_ArrCapacity = a_Capacity;
	m_ArrCount = toCopy;
}

template<typename T>
inline ArrayList<T>::ArrayList(int a_Capacity) : m_ArrCapacity(a_Capacity) {
	m_Array = new T[a_Capacity];
}

template<typename T>
inline ArrayList<T>::~ArrayList() {
	delete[] m_Array;
	m_Array = nullptr;
}

template<typename T>
inline T& ArrayList<T>::get(int a_Index) {
	assert(a_Index >= 0 && a_Index < m_ArrCount && "Index out of bounds");

	return m_Array[a_Index];
}

template<typename T>
inline void ArrayList<T>::add(const T& a_Item) {
	if (m_ArrCount >= m_ArrCapacity)
		resize(m_ArrCapacity * SCALE_MUL);

	m_Array[m_ArrCount] = a_Item;
	m_ArrCount++;
}

template<typename T>
inline void ArrayList<T>::insert(int a_Index, const T& a_Item) {
	assert(a_Index >= 0 && a_Index <= m_ArrCount && "Index out of bounds");

	if (m_ArrCount >= m_ArrCapacity)
		resize(m_ArrCapacity * SCALE_MUL);

	for (int i = m_ArrCount; i > a_Index; i--) {
		m_Array[i] = m_Array[i - 1];
	}

	m_ArrCount++;
	m_Array[a_Index] = a_Item;
}

template<typename T>
inline void ArrayList<T>::removeAt(int a_Index) {
	assert(a_Index >= 0 && a_Index < m_ArrCount && "Index out of bounds");

	for (int i = a_Index; i < m_ArrCount - 1; i++) {
		m_Array[i] = m_Array[i + 1];
	}

	m_ArrCount--;

	if (m_ArrCount > DEFAULT_ARR_CAPACITY &&
		m_ArrCount < m_ArrCapacity / (SCALE_MUL * 2)
		) {
		resize(m_ArrCapacity / SCALE_MUL);
	}
}

template<typename T>
inline void ArrayList<T>::remove(T& a_Item) {
	for (int i = 0; i < m_ArrCount; i++) {
		if (m_Array[i] == a_Item) {
			removeAt(i);
			break;
		}
	}
}

template<typename T>
inline void ArrayList<T>::removeIf(
	bool (*a_Condition)(const T&), 
	void (*a_OnRemove)(T&)
) {
	int i = 0, j = 0;

	for (; i < m_ArrCount; i++) {
		if (!a_Condition(m_Array[i])) {
			m_Array[j++] = m_Array[i];
		}
		else {
			if (a_OnRemove != nullptr)
				a_OnRemove(m_Array[i]);
		}
	}

	m_ArrCount = j;
}

template<typename T>
inline int ArrayList<T>::count() const {
	return m_ArrCount;
}

template<typename T>
inline void ArrayList<T>::clear() {
	m_ArrCount = 0;
	resize(DEFAULT_ARR_CAPACITY);
}

// Bubble sort z flag¹ swapped
template<typename T>
inline void ArrayList<T>::sort(bool(*a_Compare)(const T&, const T&)) {
	bool swapped = false;

	for (int i = 0; i < m_ArrCount - 1; i++) {
		for (int j = 0; j < m_ArrCount - i - 1; j++) {
			swapped = false;

			if (a_Compare(m_Array[j], m_Array[j + 1])) {
				T temp = m_Array[j];
				m_Array[j] = m_Array[j + 1];
				m_Array[j + 1] = temp;

				swapped = true;
			}
		}
		if (!swapped) break;
	}
}

template<typename T>
inline T& ArrayList<T>::operator[](int a_Index) {
	return get(a_Index);
}

template<typename T>
inline T* ArrayList<T>::begin()
{
	return m_Array;
}

template<typename T>
inline T* ArrayList<T>::end()
{
	return m_Array + m_ArrCount;
}

template<typename T>
inline const T* ArrayList<T>::begin() const {
	return m_Array;
}

template<typename T>
inline const T* ArrayList<T>::end() const {
	return m_Array + m_ArrCount;
}

template<typename K, typename V>
inline bool Map<K, V>::compareKeys(const K& a_Key1, const K& a_Key2) const
{
	if constexpr (isStr<K>()) {
		return strcmp(a_Key1, a_Key2) == 0;
	}
	else {
		return a_Key1 == a_Key2;
	}
}

template<typename K, typename V>
inline Map<K,V>::Map(int a_Size) : m_Array(a_Size) {}

template<typename K, typename V>
inline Map<K, V>::~Map() {
	clear();
}

template<typename K, typename V>
inline V& Map<K, V>::get(K a_Key) {
	if (a_Key == 0) {
		assert(false && "Key must not be null");
		return m_Array[0].value;
	}

	for (KVPair<K,V>& item : m_Array) {
		if (compareKeys(a_Key, item.key))
			return item.value;
	}

	assert(false && "Key does not exists");
	return m_Array[0].value;
}

template<typename K, typename V>
inline V& Map<K, V>::getByIndex(int a_Index)
{
	return m_Array.get(a_Index).value;
}

template<typename K, typename V>
inline bool Map<K, V>::containsKey(const K a_Key) const
{
	for (const KVPair<K, V>& item : m_Array) {
		if (compareKeys(a_Key, item.key))
			return true;
	}

	return false;
}

template<typename K, typename V>
inline void Map<K, V>::put(const K a_Key, const V& a_Val) {
	for (KVPair<K, V>& item : m_Array) {
		if (compareKeys(a_Key, item.key)) {
			item.value = a_Val;
			return;
		}
	}

	if constexpr (isStr<K>()) {
		m_Array.add({ copy_string(a_Key), a_Val });
	} else {
		m_Array.add({ a_Key, a_Val });
	}
}

template<typename K, typename V>
inline void Map<K, V>::remove(const K a_Key) {
	for (int i = 0; i < m_Array.count(); i++) {
		if (compareKeys(a_Key, m_Array[i].key)) {
			if constexpr (isStr<K>()) {
				delete[] m_Array[i].key;
				m_Array[i].key = nullptr;
			}
			m_Array.removeAt(i);
			return;
		}
	}
}

template<typename K, typename V>
inline void Map<K, V>::clear() {
	if constexpr (isStr<K>()) {
		for (KVPair<K, V>& item : m_Array) {
			delete[] item.key;
			item.key = nullptr;
		}
	}
	m_Array.clear();
}

template<typename K, typename V>
inline int Map<K, V>::count() const {
	return m_Array.count();
}

template<typename K, typename V>
inline const V& Map<K, V>::operator[](const K a_Key) const {
	for (const KVPair<K, V>& item : m_Array) {
		if (compareKeys(item.key, a_Key)) {
			return item.value;
		}
	}

	assert(false && "Key does not exists");
}

template<typename K, typename V>
inline V& Map<K, V>::operator[](const K a_Key) {
	for (KVPair<K, V>& item : m_Array) {
		if (compareKeys(item.key, a_Key)) {
			return item.value;
		}
	}

	if constexpr (isStr<K>())
		m_Array.add({ copy_string(a_Key), V() });
	else
		m_Array.add({ a_Key, V()});

	return m_Array[m_Array.count() - 1].value;
}

template<typename K, typename V>
inline KVPair<K, V>* Map<K, V>::begin()
{
	return m_Array.begin();
}

template<typename K, typename V>
inline KVPair<K, V>* Map<K, V>::end()
{
	return m_Array.end();
}

template<typename K, typename V>
inline const KVPair<K, V>* Map<K, V>::begin() const
{
	return m_Array.begin();
}

template<typename K, typename V>
inline const KVPair<K, V>* Map<K, V>::end() const
{
	return m_Array.end();
}

template<typename T>
inline CircularQueue<T>::CircularQueue(int a_Size)
{
	m_Array = new T[a_Size];
	m_Size = a_Size;
	m_ItemCount = 0;
	m_Read = 0;
	m_Write = 0;
}

template<typename T>
inline CircularQueue<T>::~CircularQueue()
{
	delete[] m_Array;
}

template<typename T>
inline void CircularQueue<T>::push(T a_Val)
{
	m_Array[m_Write] = a_Val;
	m_Write = (m_Write + 1) % m_Size;

	if (m_ItemCount < m_Size)
		m_ItemCount++;
	else
		m_Read = (m_Read + 1) % m_Size;
}

template<typename T>
inline T CircularQueue<T>::pop()
{
	assert(m_ItemCount > 0 && "Queue index out of bounds");

	T val = m_Array[m_Read];
	m_ItemCount--;
	m_Read = (m_Read + 1) % m_Size;
	return val;
}

template<typename T>
inline T& CircularQueue<T>::peek()
{
	return m_Array[m_Read];
}

template<typename T>
inline T& CircularQueue<T>::peekAt(int a_IdxFromRead) 
{
	assert(a_IdxFromRead < count() && "Queue index out of bounds");
	return m_Array[(m_Read + a_IdxFromRead) % m_Size];
}

template<typename T>
inline void CircularQueue<T>::clear()
{
	m_ItemCount = 0;
	m_Read = 0;
	m_Write = 0;
}

template<typename T>
inline int CircularQueue<T>::count() const
{
	return m_ItemCount;
}

template<typename T>
inline int CircularQueue<T>::maxSize() const
{
	return m_Size;
}
