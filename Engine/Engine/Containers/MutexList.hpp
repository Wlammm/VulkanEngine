#pragma once
#include "List.hpp"
#include "tracy/tracy/Tracy.hpp"
#include <mutex>

#define LockMutex std::unique_lock<std::recursive_mutex> lock; { ZoneScopedN("MutexList::Wait"); lock = std::unique_lock<std::recursive_mutex>(myMutex); }

template<typename ElementType, typename SizeType = int>
class MutexList
{
public:
	MutexList()
	{
		LockMutex;
		myList = List<ElementType, SizeType>();
	}

	MutexList(const SizeType inCapacity)
	{
		LockMutex;
		myList = List<ElementType, SizeType>(inCapacity);
	}

	MutexList(const List<ElementType>& inCopy)
	{
		LockMutex;
		myList = List<ElementType, SizeType>(inCopy);
	}

	MutexList(const MutexList<ElementType>& inCopy)
	{
		LockMutex;
		myList = List<ElementType, SizeType>(inCopy.myList);
	}

	MutexList(const std::vector<ElementType>& inCopy)
	{
		LockMutex;
		myList = List<ElementType, SizeType>(inCopy);
	}

	MutexList(const std::initializer_list<ElementType>& inInitList)
	{
		LockMutex;
		myList = List<ElementType, SizeType>(inInitList);
	}
	
	MutexList<ElementType, SizeType>& operator=(const List<ElementType>& inCopy)
	{
		LockMutex;
		myList.operator=(inCopy);
		return *this;
	}

	MutexList<ElementType, SizeType>& operator=(const MutexList<ElementType>& inCopy)
	{
		LockMutex;
		myList.operator=(inCopy.myList);
		return *this;
	}

	void Clear()
	{
		LockMutex;
		myList.Clear();
	}

	void Resize(const SizeType inSize)
	{
		LockMutex;
		myList.Resize(inSize);
	}

	void Reserve(const SizeType inSize)
	{
		LockMutex;
		myList.Reserve(inSize);
	}

	SizeType FindIndex(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		LockMutex;
		return myList.FindIndex(inValue);
	}

	bool Contains(const ElementType& inValue) const requires ComparisonOperator<ElementType>
	{
		LockMutex;
		return myList.Contains(inValue);
	}

	constexpr SizeType size() const noexcept
	{
		LockMutex;
		return myList.size();
	}

	void Add(const ElementType& inValue)
	{
		LockMutex;
		myList.Add(inValue);
	}

	void Add(ElementType&& inValue)
	{
		LockMutex;
		myList.Add(inValue);
	}

	template<typename... ArgsType>
	ElementType& Emplace(ArgsType&&... InArgs)
	{
		LockMutex;
		return myList.Emplace(std::forward<ArgsType>(InArgs)...);
	}

	void AddRange(const List<ElementType>& inList)
	{
		LockMutex;
		myList.AddRange(inList);
	}

	void RemoveIndex(const SizeType inIndex)
	{
		LockMutex;
		myList.RemoveIndex(inIndex);
	}

	void Remove(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		LockMutex;
		myList.Remove(inValue);
	}

	void RemoveRange(const SizeType inNumElements, const SizeType inOffset = 0)
	{
		LockMutex;
		myList.RemoveRange(inNumElements, inOffset);
	}

	const ElementType& operator[] (const SizeType inIndex) const
	{
		LockMutex;
		return myList.operator[](inIndex);
	}

	ElementType& operator[] (const SizeType inIndex)
	{
		LockMutex;
		return myList.operator[](inIndex);
	}

	const ElementType& First() const
	{
		LockMutex;
		return myList.First();
	}

	ElementType& First()
	{
		LockMutex;
		return myList.First();
	}

	const ElementType& Last() const
	{
		LockMutex;
		return myList.Last();
	}

	ElementType& Last()
	{
		LockMutex;
		return myList.Last();
	}

	ElementType* data() const
	{
		LockMutex;
		return myList.data();
	}

	ElementType* begin() const
	{
		LockMutex;
		return myList.begin();
	}

	ElementType* end() const
	{
		LockMutex;
		return myList.end();
	}

	// Manually lock the list until unlock is called. Use this to not have to aquire a lock for every operation.
	// Once locked list is only accessible for the thread that locked it
	void Lock()
	{
		myMutex.lock();
	}

	void Unlock()
	{
		myMutex.unlock();
	}

	std::recursive_mutex& GetMutex() const
	{
		return myMutex;
	}

private:
	mutable std::recursive_mutex myMutex;
	List<ElementType, SizeType> myList{};
};