#pragma once
#include "Engine/Reflection/Class.h"
#include "Engine/Reflection/ReflectionSystem.h"

#define private public
#define protected public

// BEGIN TEMP
class SomeBaseClass
{
    
};

class TestClass : public SomeBaseClass
{
    
private:
    int someInt;
    float* someFloatPtr;
};
// END TEMP

class GeneratedReflectionData
{
public:
    static void RegisterReflectionData(Engine& inEngine)
    {
        ReflectionSystem& reflectionSystem = inEngine.GetEngineSystem<ReflectionSystem>(); 

        // Create all classes.
        {
            reflectionSystem.AddClass("TestClass", typeid(TestClass).name());
            reflectionSystem.AddClass("SomeBaseClass", typeid(SomeBaseClass).name());
            reflectionSystem.AddClass("int", typeid(int).name());
            reflectionSystem.AddClass("float", typeid(float).name());
        }

        // Add all fields & add base classes.
        {
            {
                Class* currentClass = reflectionSystem.GetMutableClass<TestClass>();
                currentClass->AddField(Field("someInt", offsetof(TestClass, someInt), reflectionSystem.GetClass<int>(), false, false));
                currentClass->AddField(Field("someFloatPtr", offsetof(TestClass, someFloatPtr), reflectionSystem.GetClass<float>(), true, false));
                currentClass->AddBaseClass(reflectionSystem.GetMutableClass<SomeBaseClass>());
            }
        }
    }
};

#undef private
#undef protected