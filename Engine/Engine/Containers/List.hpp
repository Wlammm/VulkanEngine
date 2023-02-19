#pragma once

template<typename T>
class List
{
public:
	List()
	{
		myPtr = new T(myGrowthSize);
		myCapacity = myGrowthSize;
	}

	~List()
	{
		delete[] myPtr;
		myCapacity = 0;
		mySize = 0;
	}

	void SetGrowthSize(const int inGrowthSize)
	{
		check(inGrowthSize > 0 && "Growth size must be more than 0.");
		myGrowthSize = inGrowthSize;
	}

	void Add(const T& inValue)
	{
		
	}

private:
	T* myPtr = nullptr;
	int mySize = 0;
	int myCapacity = 0;

	int myGrowthSize = 4;
};