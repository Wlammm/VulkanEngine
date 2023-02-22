#include "UnitTestPch.h"
#include "Engine/Containers/List.hpp"

TEST(List, DefaultConstruct)
{
	List<int> list{};
	EXPECT_EQ(list.GetSize(), 0);
}

TEST(List, ConstructWithCapacity)
{
	List<int> list(10);
	EXPECT_EQ(list.GetCapacity(), 10);
}

TEST(List, CopyConstruct)
{
	List<int> other;
	other.Add(3);
	other.Add(4);
	other.Add(5);

	List<int> list(other);
	EXPECT_EQ(list.GetSize(), other.GetSize());
	EXPECT_EQ(list.GetCapacity(), other.GetCapacity());
	EXPECT_EQ(list[0], other[0]);
	EXPECT_EQ(list[1], other[1]);
	EXPECT_EQ(list[2], other[2]);
}

TEST(List, ConstructInitList)
{
	List<int> list{ 3, 4, 5, 6, 7 };
	EXPECT_EQ(list[0], 3);
	EXPECT_EQ(list[1], 4);
	EXPECT_EQ(list[2], 5);
	EXPECT_EQ(list[3], 6);
	EXPECT_EQ(list[4], 7);
}

TEST(List, EqualsOperator)
{
	List<int> other{ 1, 2, 3, 4, 5 };
	List<int> list = other;
	EXPECT_EQ(list.GetSize(), other.GetSize());
	EXPECT_EQ(list.GetCapacity(), other.GetCapacity());
	EXPECT_EQ(list[0], other[0]);
	EXPECT_EQ(list[1], other[1]);
	EXPECT_EQ(list[2], other[2]);
	EXPECT_EQ(list[3], other[3]);
	EXPECT_EQ(list[4], other[4]);
}

TEST(List, IsEmpty)
{
	List<int> list{};
	EXPECT_TRUE(list.IsEmpty());
	list.Add(3);
	EXPECT_FALSE(list.IsEmpty());
	list.Clear();
	EXPECT_TRUE(list.IsEmpty());
}

TEST(List, Clear)
{
	List<int> defaultList{};
	List<int> list{ 3, 4, 5, 7, 5, 4 };
	list.Clear();
	EXPECT_TRUE(list.IsEmpty());
	EXPECT_TRUE(list.GetCapacity() != defaultList.GetCapacity());
}

TEST(List, Add)
{
	List<int> list{};
	list.Add(0);
	list.Add(3);
	list.Add(34);
	EXPECT_EQ(list.GetSize(), 3);
	EXPECT_EQ(list[0], 0);
	EXPECT_EQ(list[1], 3);
	EXPECT_EQ(list[2], 34);
}

TEST(List, AddRange)
{
	List<int> list{2, 1, 1};
	list.AddRange({ 3, 4, 5,6, 8 });
	EXPECT_EQ(list[0], 2);
	EXPECT_EQ(list[5], 5);
	EXPECT_EQ(list.GetSize(), 8);
}

TEST(List, RemoveIndex)
{
	List<int> list{ 1, 2, 3, 4, 5 };
	list.RemoveIndex(2);
	EXPECT_EQ(list[1], 2);
	EXPECT_EQ(list[2], 4);
	EXPECT_EQ(list[3], 5);
}

TEST(List, Remove)
{
	List<int> list{ 0, 1, 2, 3, 4, 5 };
	list.Remove(3);
	EXPECT_EQ(list[3], 4);
	EXPECT_EQ(list[4], 5);
	EXPECT_EQ(list[2], 2);
}

TEST(List, Reserve)
{
	List<int> list;
	list.Reserve(10);
	EXPECT_EQ(list.GetCapacity(), 10);
}