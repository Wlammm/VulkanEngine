#pragma once

class System;

template <typename BaseSystemType>
class SystemManager
{
public:
    ~SystemManager()
    {
        // Clear the systems in reverse order to make sure we keep initialization order intact.
        for (int i = mySystems.size() - 1; i >= 0; --i)
        {
            mySystems[i].Reset();
        }
        mySystems.Clear();
    }

    void InitAllSystems()
    {
        for (const UniquePtr<BaseSystemType>& system : mySystems)
        {
            system->Init();
        }
    }

    template <typename SystemType, typename... Args>
    void AddSystem(Args&&... inArgs)
    {
        static_assert(std::is_base_of<BaseSystemType, SystemType>::value, "SystemType must derive from BaseSystemType");
        mySystems.Add(MakeUnique<SystemType>(std::forward<Args>(inArgs)...));
    }

    template <typename SystemType>
    SystemType& GetSystem()
    {
        for (const UniquePtr<BaseSystemType>& system : mySystems)
        {
            if (SystemType* castedSystem = dynamic_cast<SystemType*>(system.Get()))
            {
                return *castedSystem;
            }
        }
        check(false && "Subsystem does not exists.");
        return *(SystemType*)nullptr;
    }

    const List<UniquePtr<BaseSystemType>>& GetAllSystems() const
    {
        return mySystems;
    }

private:
    List<UniquePtr<BaseSystemType>> mySystems{};
};
