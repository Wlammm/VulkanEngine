#pragma once

class ISystem
{
public:
	virtual void Tick() = 0;
	virtual const std::unordered_set<std::string>& GetDependencies() = 0; 
};

template<typename... ComponentDependencies>
class System : ISystem
{
public:
	System()
	{
		Emplace<ComponentDependencies...>();
	}

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
		myDependencies.emplace(typeid(T).name());
	}

private:
	std::unordered_set<std::string> myDependencies{};
};