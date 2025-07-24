#pragma once

template<typename PtrType>
using SharedPtr = std::shared_ptr<PtrType>;

template<typename PtrType, typename... Args>
SharedPtr<PtrType> MakeShared(Args&&... inArgs)
{
    return std::make_shared<PtrType>(std::forward<Args>(inArgs)...);
}

template<typename PtrType>
using WeakPtr = std::weak_ptr<PtrType>;