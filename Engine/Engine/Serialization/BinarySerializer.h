#pragma once
#include "Engine/Containers/MutexList.hpp"

class BinarySerializer
{
public:
    enum class Mode
    {
        Read,
        Write
    };

    BinarySerializer(const std::filesystem::path& inPath, const Mode inMode);
    BinarySerializer(std::ostringstream& inStringStream);
    ~BinarySerializer();

    bool IsReading() const;
    bool IsWriting() const;

    void Close();

    template <typename Type>
    void SerializeCopyable(Type& inOutType)
    {
        if (IsReading())
            ReadCopyable(inOutType);
        else
            WriteCopyable(inOutType);
    }

    template <typename ClassType>
    void SerializeType(ClassType& inOutInstance)
    {
        myWasLastTypeSerializationFullyComplete = true;
        SerializeTypeInternal(inOutInstance);
    }

    void SerializeType(void* inOutInstance, const Type* inType, const bool inIsPointer);

    // Returns true if last serialization was fully complete. No fields failed to serialize.
    bool WasLastTypeSerializationFullyComplete() const;
    
    void SerializeString(std::string& inOutString);

    void SerializeBinaryData(void* inOutInstance, const int inSize);

private:
    inline static MutexList<std::string> missingTypesAlreadyWarnedAbout{};
    
    friend class TypeSerializer;
    
    template <typename ClassType>
    void SerializeTypeInternal(ClassType& inOutInstance)
    {
        void* instance = (void*)&inOutInstance;
        bool isPointer = std::is_pointer<ClassType>::value;
        static_assert(!std::is_reference_v<ClassType> && "SerializeClass does not support reference types.");
        SerializeTypeInternal(instance, typeid(std::remove_pointer_t<ClassType>).name(), isPointer);
    }

    // This is using std::string and typeID instead of Type* because we need to break circular include with asset registry and reflection system.
    void SerializeTypeInternal(void* inOutInstance, const std::string& inTypeId, const bool inIsPointer);
    
    void ReadType(void* outInstance, const Type* inType, const bool inIsPointer);
    void WriteType(void* inInstance, const Type* inType, const bool inIsPointer);

    void WriteBinaryData(void* inInstance, const int inSize);
    void ReadBinaryData(void* outInstance, const int inSize);

    template <typename Type>
    void ReadCopyable(Type& outType)
    {
        memcpy(&outType, myInBuffer.data() + myReadOffset, sizeof(Type));
        myReadOffset += sizeof(Type);
    }

    template <typename Type>
    void WriteCopyable(Type& inType)
    {
        char* c = (char*)&inType;
        size_t size = sizeof(Type);
        myOutputStream->write(c, size);
    }

    void WriteString(const std::string& inString);
    void ReadString(std::string& outString);

private:
    std::filesystem::path myPath;
    std::ostream* myOutputStream = nullptr;

    bool myIsStreamOpen = false;
    bool myIsTemporary = false;

    List<unsigned char> myInBuffer;
    size_t myReadOffset = 0;

    // Returns true if all fields were serialized correctly.
    bool myWasLastTypeSerializationFullyComplete = true;

    Mode myMode;
};
