#pragma once

class ISystem
{
public:
	virtual ~ISystem() {}

	virtual void Tick() = 0;
	virtual const std::unordered_set<std::string>& GetDependencies() = 0; 
};

template<typename... Dependencies>
class System : public ISystem
{
public:
	System()
	{
		myDependencies = { GetTypeName<Dependencies>()... };
	}

	virtual ~System() {}

	virtual const std::unordered_set<std::string>& GetDependencies() override final
	{
		return myDependencies;
	}

private:
	template<typename T>
	std::string GetTypeName()
	{
		std::string returnVal = "";
		if constexpr (std::is_const<T>::value)
			returnVal += "const ";

		return returnVal + typeid(T).name();
	}
	
private:
	std::unordered_set<std::string> myDependencies{};
};