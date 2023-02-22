#include "Engine/Containers/List.hpp"

TEST(List, Construct)
{
	List<int> list;
	EXPECT_EQ(list.GetSize(), 0);
	list.Add(3);
	list.Add(3);
	list.Add(3);
	list.Add(3);
	list.Add(3);
	list = List<int>();
	EXPECT_EQ(list.GetSize(), 0);
}

TEST(List, Add)
{
	List<int> list;
	list.Add(3);
	list.Add(4);
	list.Add(5);
	list.Add(36);
	EXPECT_EQ(list.GetSize(), 4);
}