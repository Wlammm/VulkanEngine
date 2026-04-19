#pragma once
#include <vector>
#include <External/tracy/tracy/Tracy.hpp>

#include "ContainerTypes.hpp"

#define CanCopy std::is_trivially_copyable<ElementType>::value || IsCopyable<ElementType>::value

template<typename ElementType>
concept ComparisonOperator = requires(ElementType a)
{
	a == a;
};

using ListSizeType = int;

class IList
{
public:
	virtual ~IList() = default;
	virtual constexpr ListSizeType size() const noexcept = 0;
	virtual int GetSizeofElement() const = 0;
	virtual void Resize(const ListSizeType inSize) = 0;
	virtual void Clear() = 0;
	
	virtual void* GetData() const = 0;
};

template<typename ElementType>
class List final : public IList
{
public:
	List()
	{
		Construct();
	}

	List(const ListSizeType inCapacity)
	{
		Construct();
		Grow(inCapacity);
	}

	List(const List<ElementType>& inCopy) requires std::is_copy_constructible_v<ElementType>
	{
		Construct();
		AddRange(inCopy);
	}

	List(const std::vector<ElementType>& inCopy)
	{
		Construct();
		check(inCopy.capacity() < std::numeric_limits<ListSizeType>::max());
		Grow(static_cast<ListSizeType>(inCopy.capacity()));

		if constexpr (CanCopy)
		{
			mySize = static_cast<ListSizeType>(inCopy.size());
			memcpy(myPtr, inCopy.data(), sizeof(ElementType) * inCopy.size());
		}
		else
		{
			for (int i = 0; i < inCopy.size(); ++i)
			{
				myPtr[i] = inCopy[i];
			}

			mySize = inCopy.size();
		}
	}

	List(List&& other)
		: myPtr(std::exchange(other.myPtr, nullptr))
		, mySize(std::exchange(other.mySize, 0))
		, myCapacity(std::exchange(other.myCapacity, 0))
	{
	}

	List& operator=(List&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			free(myPtr);

			myPtr = std::exchange(other.myPtr, nullptr);
			mySize = std::exchange(other.mySize, 0);
			myCapacity = std::exchange(other.myCapacity, 0);
		}
		return *this;
	}
	
	bool operator==(const List& inOther) const
	{
		if (this == &inOther)
			return true;

		if (mySize != inOther.mySize)
			return false;
		
		return std::equal(begin(), end(), inOther.begin());
	}

	List(const std::initializer_list<ElementType>& inInitList) requires std::is_copy_constructible_v<ElementType>
	{
		Construct();

		for (const auto& value : inInitList)
		{
			Add(value);
		}
	}

	virtual ~List()
	{
		Clear();
		if (myPtr)
		{
			free((void*)myPtr);
			myPtr = nullptr;
		}
		myCapacity = 0;
		mySize = 0;
	}

	void operator=(const List<ElementType>& inCopy) requires std::is_copy_constructible_v<ElementType>
	{
		Clear();
		AddRange(inCopy);
	}

	constexpr ListSizeType size() const noexcept override
	{
		return mySize;
	}

	ListSizeType capacity() const noexcept
	{
		return myCapacity;
	}

	bool IsEmpty() const
	{
		return mySize == 0;
	}

	bool IsValidIndex(const ListSizeType inIndex) const
	{
		return inIndex >= 0 && inIndex < mySize;
	}

	void Clear() override
	{
		if constexpr(!std::is_trivially_destructible<ElementType>::value)
		{
			for (const auto& value : *this)
			{
				value.~ElementType();
			}
		}

		mySize = 0;
	}

	void Resize(const ListSizeType inSize) override
	{
		if(mySize == inSize)
			return;
		
		if (inSize < mySize)
		{
			if constexpr (!std::is_trivially_destructible_v<ElementType>)
			{
				for (ListSizeType i = inSize; i < mySize; ++i)
				{
					myPtr[i].~ElementType();
				}
			}

			mySize = inSize;
		}

		Grow(inSize);
		mySize = inSize;
	}

	void Reserve(const ListSizeType inSize)
	{
		Grow(inSize);
	}

	ListSizeType FindIndex(const ElementType& inValue) const requires ComparisonOperator<ElementType>
	{
		for(ListSizeType i = 0; i < mySize; ++i)
		{
			if (myPtr[i] == inValue)
				return i;
		}
		return -1;
	}

	void Reset()
	{
		Clear();
		mySize = 0;
		free(myPtr);
		myPtr = nullptr;
		Construct();
	}

	bool Contains(const ElementType& inValue) const requires ComparisonOperator<ElementType>
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

	void AddRange(const List<ElementType>& inList) requires std::is_copy_constructible_v<ElementType>
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
				new (myPtr + mySize) ElementType(value);
				mySize++;
			}
		}
	}

	void AddRange(List<ElementType>&& inList) requires std::is_move_constructible_v<ElementType>
	{
		CheckCapacityForAdd(inList.size());

		for (ElementType& value : inList)
		{
			new (myPtr + mySize) ElementType(std::move(value));
			++mySize;
		}
		inList.Clear();
	}
#pragma endregion

#pragma region Remove
	void RemoveIndex(const ListSizeType inIndex)
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");

		if constexpr(!std::is_trivially_destructible<ElementType>::value)
		{
			myPtr[inIndex].~ElementType();
		}

		if constexpr (CanCopy)
		{
			const size_t elementsToMove = mySize - inIndex - 1;
			if (elementsToMove > 0)
			{
				memcpy(&myPtr[inIndex], &myPtr[inIndex + 1], sizeof(ElementType) * elementsToMove);
			}
		}
		else
		{
			for (ListSizeType i = inIndex; i < mySize - 1; ++i)
			{
				new (myPtr + i) ElementType(std::move(myPtr[i + 1]));
			}
		}
		mySize--;
	}

	void RemoveLast()
	{
		RemoveIndex(mySize - 1);
	}

	void Remove(const ElementType& inValue) requires ComparisonOperator<ElementType>
	{
		for (ListSizeType i = 0; i < mySize; ++i)
		{
			if (myPtr[i] == inValue)
			{
				RemoveIndex(i);
				return;
			}
		}
	}

	void RemoveRange(const ListSizeType inNumElements, const ListSizeType inOffset = 0)
	{
		// TODO: Improve performance by bulk removing here.
		for(ListSizeType i = 0; i  < inNumElements; ++i)
		{
			RemoveIndex(inOffset);
		}
	}
#pragma endregion

#pragma region Data Accessors
	const ElementType& operator[] (const ListSizeType inIndex) const
	{
		check(inIndex >= 0 && inIndex < mySize && "Index out of range.");
		return myPtr[inIndex];
	}

	ElementType& operator[] (const ListSizeType inIndex)
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

	void* GetData() const override
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

	int GetSizeofElement() const override
	{
		return sizeof(ElementType);
	}

private:
	ListSizeType UpperPowerOfTwo(ListSizeType inRequiredSize)
	{
		ListSizeType a = static_cast<ListSizeType>(log2(inRequiredSize));

		if (pow(2, a) == inRequiredSize)
			return inRequiredSize;

		return static_cast<ListSizeType>(pow(2, a + 1));
	}

	void Construct()
	{
		Grow(2);
	}

	void CheckCapacityForAdd(const ListSizeType inNumNewElements)
	{
		const ListSizeType requiredSize = mySize + inNumNewElements;
		if (requiredSize <= myCapacity)
			return;

		const ListSizeType newSize = UpperPowerOfTwo(requiredSize);
		Grow(newSize);
	}

	void Grow(const ListSizeType inNewCapacity)
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
				for (ListSizeType i = 0; i < mySize; ++i)
				{
					new (myPtr + i) ElementType(std::move(oldPtr[i]));
					oldPtr[i].~ElementType();
				}
			}
			free(oldPtr);
		}
	}

private:
	ElementType* myPtr = nullptr;
	ListSizeType mySize = 0;
	ListSizeType myCapacity = 0;
};