#pragma once

#define COMP(ComponentName)\
\
public:\
    const std::string& GetComponentName() const override\
    {\
        static std::string compName = #ComponentName;\
        return compName;\
    }\
\
private: