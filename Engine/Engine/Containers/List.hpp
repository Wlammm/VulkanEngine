#pragma once

template<typename T, bool allowCopy = true>
class List
{
public:
	List()
	{
		Construct();
	}

	List(const uint inCapacity)
	{
		Construct();
		Grow(inCapacity);
	}

	List(const List<T>& inCopy)
	{
		Construct();
		AddRange(inCopy);
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
		delete[] myPtr;
		myCapacity = 0;
		mySize = 0;
	}

	void SetGrowthMultiplier(const uint inGrowthMultiplier)
	{
		check(inGrowthMultiplier > 0 && "Growth size must be more than 0.");
		myGrowthMultiplier = inGrowthMultiplier;
	}

	uint GetSize() const
	{
		return mySize;
	}

	uint GetCapacity() const
	{
		return myCapacity;
	}

	bool IsEmpty() const
	{
		return mySize == 0;
	}

	void Clear()
	{
		mySize = 0;
	}

	void Reserve(const uint inSize)
	{
		Grow(inSize);
	}

#pragma region Add
	void Add(const T& inValue)
	{
		CheckCapacityForAdd(1);
		myPtr[mySize] = inValue;
		mySize++;
	}

	void AddRange(const List<T>& inList)
	{
		CheckCapacityForAdd(inList.GetSize());

		if (CanCopy())
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
	void RemoveIndex(const uint inIndex)
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");

		if (CanCopy())
		{
			memcpy(&myPtr[inIndex], &myPtr[inIndex + 1], sizeof(T) * mySize - inIndex);
		}
		else
		{
			for (uint i = inIndex; i < mySize - 1; ++i)
			{
				myPtr[i] = myPtr[i + 1];
			}
		}
		mySize--;
	}

	void Remove(const T& inValue)
	{
		for (uint i = 0; i < mySize; ++i)
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
	const T& operator[] (const uint inIndex) const
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	T& operator[] (const uint inIndex)
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

	const T* Data() const
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
		myPtr = new T[myGrowthMultiplier];
		myCapacity = myGrowthMultiplier;
	}

	void CheckCapacityForAdd(const uint inNumNewElements)
	{
		if (mySize + inNumNewElements <= myCapacity)
			return;

		Grow(mySize + inNumNewElements);
	}

	void Grow(const uint inRequiredCapacity)
	{
		uint newCapacity = myCapacity * 2;
		while (newCapacity < inRequiredCapacity)
			newCapacity *= myGrowthMultiplier;

		T* oldPtr = myPtr;
		myPtr = new T[newCapacity];
		
		if(CanCopy())
		{
			memcpy(myPtr, oldPtr, sizeof(T) * mySize);
		}
		else
		{
			for (uint i = 0; i < mySize; ++i)
			{
				myPtr[i] = oldPtr[i];
			}
		}
		
		myCapacity = newCapacity;

		delete[] oldPtr;
	}

	__forceinline constexpr bool CanCopy() const
	{
		return allowCopy && std::is_trivially_copyable<T>::value;
	}

private:
	T* myPtr = nullptr;
	uint mySize = 0;
	uint myCapacity = 0;

	uint myGrowthMultiplier = 2;
};