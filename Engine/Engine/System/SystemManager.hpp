#pragma once

class System;

template<typename BaseSystemType>
class SystemManager
{
public:
    ~SystemManager()
    {
        for(System* system : mySystems)
        {
            del(system);
        }
        mySystems.Clear();
    }

    void InitAllSystems()
    {
        for(System* system : mySystems)
        {
            system->Init();
        }
    }
    
    template<typename SystemType, typename... Args>
    void AddSystem(Args&&... inArgs)
    {
        static_assert(std::is_base_of<BaseSystemType, SystemType>::value, "SystemType must derive from BaseSystemType");
        SystemType* system = new SystemType(std::forward<Args>(inArgs)...);
        mySystems.Add(system);
    }
    
    template<typename SystemType>
    SystemType& GetSystem()
    {
        for(BaseSystemType* system : mySystems)
        {
            if(SystemType* castedSystem = dynamic_cast<SystemType*>(system))
            {
                return *castedSystem;
            }
        }
        check(false && "Subsystem does not exists.");
        return *(SystemType*)nullptr;
    }

    const List<BaseSystemType*> GetAllSystems() const
    {
        return mySystems;
    }

private:
    List<BaseSystemType*> mySystems{};
};
