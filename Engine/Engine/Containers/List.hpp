#pragma once
#include "ContainerTypes.hpp"

#define CanCopy std::is_trivially_copyable<T>::value || IsCopyable<T>::value

template<typename T>
concept ComparisonOperator = requires(T a)
{
	a == a;
};

template<typename T, typename SizeType = size_t>
class List
{
public:
	List()
	{
		Construct();
	}

	List(const SizeType inCapacity)
	{
		Construct();
		Grow(inCapacity);
	}

	List(const List<T>& inCopy)
	{
		Construct();
		AddRange(inCopy);
	}

	List(const std::vector<T>& inCopy)
	{
		Construct();
		Grow(inCopy.capacity());

		if constexpr (CanCopy)
		{
			mySize = static_cast<SizeType>(inCopy.size());
			memcpy(myPtr, inCopy.data(), sizeof(T) * inCopy.size());
		}
		else
		{
			for (int i = 0; i < inCopy.size(); ++i)
			{
				myPtr[i] = inCopy[i];
			}
		}
	}

	List(const std::initializer_list<T>& inInitList)
	{
		Construct();

		for (const auto& value : inInitList)
		{
			Add(value);
		}
	}

	~List()
	{
		Clear();
		if (myPtr)
		{
			free(myPtr);
			myPtr = nullptr;
		}
		myCapacity = 0;
		mySize = 0;
	}

	void operator=(const List<T>& inCopy)
	{
		Clear();
		AddRange(inCopy);
	}

	void operator=(const std::vector<T>& inCopy)
	{
		Clear();
		Grow(static_cast<SizeType>(inCopy.capacity()));

		if constexpr (CanCopy)
		{
			mySize = static_cast<SizeType>(inCopy.size());
			memcpy(myPtr, inCopy.data(), sizeof(T) * inCopy.size());
		}
		else
		{
			for(int i = 0; i < inCopy.size(); ++i)
			{
				myPtr[i] = inCopy[i];
			}
		}
	}

	void SetGrowthMultiplier(const SizeType inGrowthMultiplier)
	{
		check(inGrowthMultiplier > 0 && "Growth size must be more than 0.");
		myGrowthMultiplier = inGrowthMultiplier;
	}

	SizeType size() const noexcept
	{
		return mySize;
	}

	SizeType capacity() const noexcept
	{
		return myCapacity;
	}

	bool IsEmpty() const
	{
		return mySize == 0;
	}

	void Clear()
	{
		for (const auto& value : *this)
		{
			value.~T();
		}

		mySize = 0;
	}

	void Reserve(const SizeType inSize)
	{
		Grow(inSize);
	}

	bool Contains(const T& inValue)
	{
		for(const T& value : *this)
		{
			if (value == inValue)
				return true;
		}
		return false;
	}

#pragma region Add
	void Add(const T& inValue)
	{
		CheckCapacityForAdd(1);
		new(myPtr + mySize) T();
		myPtr[mySize] = inValue;
		mySize++;
	}

	void Add(T&& inValue)
	{
		CheckCapacityForAdd(1);
		new(myPtr + mySize) T();
		myPtr[mySize] = std::move(inValue);
		mySize++;
	}

	T& Add()
	{
		CheckCapacityForAdd(1);
		mySize++;
		new(myPtr + mySize - 1) T();
		return myPtr[mySize - 1];
	}

	void AddRange(const List<T>& inList)
	{
		CheckCapacityForAdd(inList.size());

		if constexpr (CanCopy)
		{
			memcpy(&myPtr[mySize], inList.myPtr, sizeof(T) * inList.mySize);
			mySize += inList.mySize;
		}
		else
		{
			for (const T& value : inList)
			{
				myPtr[mySize] = value;
				mySize++;
			}
		}
	}
#pragma endregion

#pragma region Remove
	void RemoveIndex(const SizeType inIndex)
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");

		myPtr[inIndex].~T();

		if constexpr (CanCopy)
		{
			memcpy(&myPtr[inIndex], &myPtr[inIndex + 1], sizeof(T) * mySize - inIndex);
		}
		else
		{
			for (SizeType i = inIndex; i < mySize - 1; ++i)
			{
				myPtr[i] = myPtr[i + 1];
			}
		}
		mySize--;
	}

	void Remove(const T& inValue) requires ComparisonOperator<T>
	{
		for (SizeType i = 0; i < mySize; ++i)
		{
			if (myPtr[i] == inValue)
			{
				RemoveIndex(i);
				return;
			}
		}
	}
#pragma endregion

#pragma region Data Accessors
	const T& operator[] (const SizeType inIndex) const
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	T& operator[] (const SizeType inIndex)
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	const T& First() const
	{
		check(!IsEmpty());
		return myPtr[0];
	}

	T& First()
	{
		check(!IsEmpty());
		return myPtr[0];
	}

	const T& Last() const
	{
		check(!IsEmpty());
		return myPtr[mySize - 1];
	}

	T& Last()
	{
		check(!IsEmpty());
		return myPtr[mySize - 1];
	}

	T* data() const
	{
		return myPtr;
	}

#pragma endregion

#pragma region Iterators
	T* begin() const
	{
		return myPtr;
	}

	T* end() const
	{
		return myPtr + mySize;
	}
#pragma endregion

private:
	void Construct()
	{
		Grow(myGrowthMultiplier);
		myCapacity = myGrowthMultiplier;
	}

	void CheckCapacityForAdd(const SizeType inNumNewElements)
	{
		const SizeType requiredSize = mySize + inNumNewElements;
		if (requiredSize <= myCapacity)
			return;

		SizeType newSize = myCapacity;
		while (newSize < requiredSize)
			newSize *= 2;

		Grow(newSize);
	}

	void Grow(const SizeType inNewCapacity)
	{
		T* oldPtr = myPtr;
		myPtr = reinterpret_cast<T*>(malloc(sizeof(T) * inNewCapacity));
		
		if constexpr (CanCopy)
		{
			memcpy(myPtr, oldPtr, sizeof(T) * mySize);
		}
		else
		{
			for (SizeType i = 0; i < mySize; ++i)
			{
				myPtr[i] = std::move(oldPtr[i]);
			}
		}
		
		myCapacity = inNewCapacity;

		if(oldPtr)
		{
			free(oldPtr);
			oldPtr = nullptr;
		}
	}

private:
	T* myPtr = nullptr;
	SizeType mySize = 0;
	SizeType myCapacity = 0;

	SizeType myGrowthMultiplier = 2;
};