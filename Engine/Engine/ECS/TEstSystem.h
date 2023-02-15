#include "System.h"

class TestSystem : public System<float, int, bool>
{
public:
	TestSystem(int a);

	void Tick() override final;

private:
	int myint;
};