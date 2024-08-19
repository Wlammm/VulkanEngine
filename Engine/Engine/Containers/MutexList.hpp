#pragma once
#include "List.hpp"
#include "tracy/tracy/Tracy.hpp"

#define LockMutex std::unique_lock<std::mutex> lock = (myManualLockID == std::thread::id() || myManualLockID != std::this_thread::get_id()) ? std::unique_lock<std::mutex>(myMutex) : std::unique_lock<std::mutex>(); ZoneScopedN("MutexList::Wait")

template<typename ElementType, typename SizeType = int>
class MutexList : public List<ElementType, SizeType>
{
public:
	MutexList()
	{
		LockMutex;
		List<ElementType, SizeType>::List();
	}

	MutexList(const SizeType inCapacity)
	{
		LockMutex;
		List<ElementType, SizeType>::List(inCapacity);
	}

	MutexList(const List<ElementType>& inCopy)
	{
		LockMutex;
		List<ElementType, SizeType>::List(inCopy);
	}

	MutexList(const std::vector<ElementType>& inCopy)
	{
		LockMutex;
		List<ElementType, SizeType>::List(inCopy);
	}

	MutexList(const std::initializer_list<ElementType>& inInitList)
	{
		LockMutex;
		List<ElementType, SizeType>::List(inInitList);
	}
	
	void operator=(const List<ElementType>& inCopy)
	{
		LockMutex;
		List<ElementType, SizeType>::operator=(inCopy);
	}

	void Clear()
	{
		LockMutex;
		List<ElementType, SizeType>::Clear();
	}

	void Resize(const SizeType inSize)
	{
		LockMutex;
		List<ElementType, SizeType>::Resize(inSize);
	}

	void Reserve(const SizeType inSize)
	{
		LockMutex;
		List<ElementType, SizeType>::Reserve(inSize);
	}

	SizeType FindIndex(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		LockMutex;
		return List<ElementType, SizeType>::FindIndex(inValue);
	}

	bool Contains(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		LockMutex;
		return List<ElementType, SizeType>::Contains(inValue);
	}

	void Add(const ElementType& inValue)
	{
		LockMutex;
		List<ElementType, SizeType>::Add(inValue);
	}

	void Add(ElementType&& inValue)
	{
		LockMutex;
		List<ElementType, SizeType>::Add(inValue);
	}

	template<typename... ArgsType>
	ElementType& Emplace(ArgsType&&... InArgs)
	{
		LockMutex;
		return List<ElementType, SizeType>::Emplace(std::forward<ArgsType>(InArgs)...);
	}

	void AddRange(const List<ElementType>& inList)
	{
		LockMutex;
		List<ElementType, SizeType>::AddRange(inList);
	}

	void RemoveIndex(const SizeType inIndex)
	{
		LockMutex;
		List<ElementType, SizeType>::RemoveIndex(inIndex);
	}

	void Remove(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		LockMutex;
		List<ElementType, SizeType>::Remove(inValue);
	}

	void RemoveRange(const SizeType inNumElements, const SizeType inOffset = 0)
	{
		LockMutex;
		List<ElementType, SizeType>::RemoveRange(inNumElements, inOffset);
	}

	const ElementType& operator[] (const SizeType inIndex) const
	{
		LockMutex;
		return List<ElementType, SizeType>::operator[](inIndex);
	}

	ElementType& operator[] (const SizeType inIndex)
	{
		LockMutex;
		return List<ElementType, SizeType>::operator[](inIndex);
	}

	const ElementType& First() const
	{
		LockMutex;
		return List<ElementType, SizeType>::First();
	}

	ElementType& First()
	{
		LockMutex;
		return List<ElementType, SizeType>::First();
	}

	const ElementType& Last() const
	{
		LockMutex;
		return List<ElementType, SizeType>::Last();
	}

	ElementType& Last()
	{
		LockMutex;
		return List<ElementType, SizeType>::Last();
	}

	ElementType* data() const
	{
		LockMutex;
		return List<ElementType, SizeType>::data();
	}

	ElementType* begin() const
	{
		LockMutex;
		return List<ElementType, SizeType>::begin();
	}

	ElementType* end() const
	{
		LockMutex;
		return List<ElementType, SizeType>::end();
	}

	// Manually lock the list until unlock is called. Use this to not have to aquire a lock for every operation.
	// Once locked list is only accessible for the thread that locked it
	void Lock()
	{
		myMutex.lock();
		myManualLockID = std::this_thread::get_id();
	}

	void Unlock()
	{
		myManualLockID = std::thread::id();
		myMutex.unlock();
	}

private:
	mutable std::mutex myMutex;
	std::thread::id myManualLockID;
};