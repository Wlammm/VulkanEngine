#pragma once
#include "ContainerTypes.hpp"

#define CanCopy std::is_trivially_copyable<ElementType>::value || IsCopyable<ElementType>::value

template<typename ElementType>
concept ComparisonOperator = requires(ElementType a)
{
	a == a;
};

template<typename ElementType, typename SizeType = size_t>
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

	List(const List<ElementType>& inCopy)
	{
		Construct();
		AddRange(inCopy);
	}

	List(const std::vector<ElementType>& inCopy)
	{
		Construct();
		Grow(inCopy.capacity());

		if constexpr (CanCopy)
		{
			mySize = static_cast<SizeType>(inCopy.size());
			memcpy(myPtr, inCopy.data(), sizeof(ElementType) * inCopy.size());
		}
		else
		{
			for (int i = 0; i < inCopy.size(); ++i)
			{
				myPtr[i] = inCopy[i];
			}
		}
	}

	List(const std::initializer_list<ElementType>& inInitList)
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

	void operator=(const List<ElementType>& inCopy)
	{
		Clear();
		AddRange(inCopy);
	}

	//void operator=(const std::vector<ElementType>& inCopy)
	//{
	//	Clear();
	//	Grow(static_cast<SizeType>(inCopy.capacity()));
	//
	//	if constexpr (CanCopy)
	//	{
	//		mySize = static_cast<SizeType>(inCopy.size());
	//		memcpy(myPtr, inCopy.data(), sizeof(ElementType) * inCopy.size());
	//	}
	//	else
	//	{
	//		for(int i = 0; i < inCopy.size(); ++i)
	//		{
	//			myPtr[i] = inCopy[i];
	//		}
	//	}
	//}

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
			value.~ElementType();
		}

		mySize = 0;
	}

	void Resize(const SizeType inSize)
	{
		Grow(inSize);
		mySize = inSize;
	}

	void Reserve(const SizeType inSize)
	{
		Grow(inSize);
	}

	SizeType FindIndex(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		for(SizeType i = 0; i < mySize; ++i)
		{
			if (myPtr[i] == inValue)
				return i;
		}
		return -1;
	}

	bool Contains(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		for (const ElementType& value : *this)
		{
			if (value == inValue)
				return true;
		}
		return false;
	}

#pragma region Add
	void Add(const ElementType& inValue)
	{
		Emplace(inValue);
	}

	void Add(ElementType&& inValue)
	{
		Emplace(std::move(inValue));
	}

	template<typename... ArgsType>
	ElementType& Emplace(ArgsType&&... InArgs)
	{
		CheckCapacityForAdd(1);
		new (myPtr + mySize) ElementType(std::forward<ArgsType>(InArgs)...);
		mySize++;
		return Last();
	}

	void AddRange(const List<ElementType>& inList)
	{
		CheckCapacityForAdd(inList.size());

		if constexpr (CanCopy)
		{
			memcpy(&myPtr[mySize], inList.myPtr, sizeof(ElementType) * inList.mySize);
			mySize += inList.mySize;
		}
		else
		{
			for (const ElementType& value : inList)
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

		myPtr[inIndex].~ElementType();

		if constexpr (CanCopy)
		{
			memcpy(&myPtr[inIndex], &myPtr[inIndex + 1], sizeof(ElementType) * (mySize - inIndex));
		}
		else
		{
			for (SizeType i = inIndex; i < mySize - 1; ++i)
			{
				new (myPtr + i) ElementType(std::move(myPtr[i + 1]));
			}
		}
		mySize--;
	}

	void Remove(const ElementType& inValue) requires ComparisonOperator<ElementType>
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

	void RemoveRange(const SizeType inNumElements, const SizeType inOffset = 0)
	{
		// TODO: Improve performance by bulk removing here.
		for(SizeType i = 0; i  < inNumElements; ++i)
		{
			RemoveIndex(inOffset);
		}
	}
#pragma endregion

#pragma region Data Accessors
	const ElementType& operator[] (const SizeType inIndex) const
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	ElementType& operator[] (const SizeType inIndex)
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	const ElementType& First() const
	{
		check(!IsEmpty());
		return myPtr[0];
	}

	ElementType& First()
	{
		check(!IsEmpty());
		return myPtr[0];
	}

	const ElementType& Last() const
	{
		check(!IsEmpty());
		return myPtr[mySize - 1];
	}

	ElementType& Last()
	{
		check(!IsEmpty());
		return myPtr[mySize - 1];
	}

	ElementType* data() const
	{
		return myPtr;
	}

#pragma endregion

#pragma region Iterators
	ElementType* begin() const
	{
		return myPtr;
	}

	ElementType* end() const
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
		if (newSize == 0)
			newSize = 1;

		while (newSize < requiredSize)
			newSize *= 2;

		Grow(newSize);
	}

	void Grow(const SizeType inNewCapacity)
	{
		ElementType* oldPtr = myPtr;
		myPtr = reinterpret_cast<ElementType*>(calloc(inNewCapacity, sizeof(ElementType)));
		myCapacity = inNewCapacity;

		if (oldPtr)
		{
			if constexpr (CanCopy)
			{
				memcpy(myPtr, oldPtr, sizeof(ElementType) * mySize);
			}
			else
			{
				for (SizeType i = 0; i < mySize; ++i)
				{
					new (myPtr + i) ElementType(std::move(oldPtr[i]));
				}
			}

			free(oldPtr);
			oldPtr = nullptr;
		}
	}

private:
	ElementType* myPtr = nullptr;
	SizeType mySize = 0;
	SizeType myCapacity = 0;

	SizeType myGrowthMultiplier = 2;
};