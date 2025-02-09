#pragma once

// Anything deriving from this needs to call SetDependencies<const SomeClass, SomeOtherClass> before next tick. Preferably in constructor
class System
{
public:
	virtual ~System() {}

	virtual void Tick() {};
	
	bool IsInitializedCorrectly() const { return myHasSetDependencies; }

	const std::unordered_set<std::string>& GetDependencies()
	{
		return myDependencies;
	}

protected:
	template<typename... Dependencies>
	void SetDependencies()
	{
		myHasSetDependencies = true;
		myDependencies = { GetTypeName<Dependencies>()... };
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
	bool myHasSetDependencies = false;
	std::unordered_set<std::string> myDependencies{};
};