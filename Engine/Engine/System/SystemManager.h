#pragma once

class System;

class SystemManager
{
public:
    ~SystemManager();

    void InitAllSystems();
    
    template<typename SystemType, typename... Args>
    void AddSystem(Args&&... inArgs)
    {
        SystemType* system = new SystemType(std::forward<Args>(inArgs)...);
        mySystems.Add(system);
    }
    
    template<typename SystemType>
    SystemType& GetSystem()
    {
        for(System* system : mySystems)
        {
            if(SystemType* castedSystem = dynamic_cast<SystemType*>(system))
            {
                return *castedSystem;
            }
        }
        check(false && "Subsystem does not exists.");
        return *(SystemType*)nullptr;
    }

private:
    List<System*> mySystems{};
};
