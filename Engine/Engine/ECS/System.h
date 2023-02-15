#pragma once

class ISystem
{
public:
	virtual ~ISystem() {}

	virtual void Tick() = 0;
	virtual const std::unordered_set<std::string>& GetDependencies() = 0; 
};

// Use this if you dont have any ComponentDependencies.
class None {};

template<typename... ComponentDependencies>
class System : public ISystem
{
public:
	System()
	{
		Emplace<ComponentDependencies...>();
	}

	virtual ~System() {}

	virtual const std::unordered_set<std::string>& GetDependencies() override final
	{
		return myDependencies;
	}

private:
	template<typename Head, typename Mid, typename... Tail>
	void Emplace()
	{
		Emplace<Head>();
		Emplace<Mid>();
		Emplace<Tail...>();
	}

	template<typename Head, typename Tail>
	void Emplace()
	{
		Emplace<Head>();
		Emplace<Tail>();
	}

	template<typename T>
	void Emplace()
	{
		if (typeid(T).name() == typeid(None).name())
			return;

		myDependencies.emplace(typeid(T).name());
	}

private:
	std::unordered_set<std::string> myDependencies{};
};