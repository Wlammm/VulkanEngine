#pragma once

class System;

class SystemManager
{
public:
    ~SystemManager();
    
    template<typename SystemType, typename... Args>
    void AddSystem(Args&&... inArgs)
    {
        SystemType* system = new SystemType(std::forward<Args>(inArgs)...);
        mySubsystems.Add(system);
        system->Init();
    }
    
    template<typename SystemType>
    SystemType& GetSystem()
    {
        for(System* system : mySubsystems)
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
    List<System*> mySubsystems{};
};
