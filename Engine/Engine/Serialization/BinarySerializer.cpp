#include "EnginePch.h"
#include "BinarySerializer.h"

BinarySerializer::BinarySerializer(const std::filesystem::path& inPath, const Mode inMode)
{
    myMode = inMode;
    myPath = inPath;

    if (IsWriting())
    {
        myIsStreamOpen = true;
        if (!std::filesystem::exists(inPath.parent_path()))
            std::filesystem::create_directories(inPath.parent_path());
        myOutputStream = new std::ofstream(myPath,std::ios::binary);
    }
    else
    {
        check(std::filesystem::exists(inPath));

        const size_t fileSize = std::filesystem::file_size(inPath);
        check(fileSize < std::numeric_limits<int>::max());
        myInBuffer.Resize(static_cast<int>(fileSize));

        FILE* file;
        errno_t error = fopen_s(&file, inPath.string().c_str(), "rb");
        check(!error);

        fread(myInBuffer.data(), sizeof(char), fileSize, file);
        fclose(file);
    }
}

BinarySerializer::BinarySerializer(std::ostringstream& inStringStream)
{
    myMode = Mode::Write;
    myOutputStream = &inStringStream;
    myIsTemporary = true;
}

BinarySerializer::~BinarySerializer()
{
    if (!myIsTemporary && myIsStreamOpen)
    {
        Close();
        del(myOutputStream);
    }
}

bool BinarySerializer::IsReading() const
{
    return myMode == Mode::Read;
}

bool BinarySerializer::IsWriting() const
{
    return myMode == Mode::Write;
}

void BinarySerializer::Close()
{
    if (IsWriting() && myIsStreamOpen && !myIsTemporary && myOutputStream)
        static_cast<std::ofstream*>(myOutputStream)->close();
    myIsStreamOpen = false;
}

void BinarySerializer::SerializeTypeInternal(void* inOutInstance, const Type* inType, const bool inIsPointer)
{
    if (IsReading())
        ReadType(inOutInstance, inType, inIsPointer);
    else
        WriteType(inOutInstance, inType, inIsPointer);
}

void BinarySerializer::SerializeType(void* inOutInstance, const Type* inType, const bool inIsPointer)
{
    myWasLastTypeSerializationFullyComplete = true;
    SerializeTypeInternal(inOutInstance, inType, inIsPointer);
}

bool BinarySerializer::WasLastTypeSerializationFullyComplete() const
{
    return myWasLastTypeSerializationFullyComplete;
}

void BinarySerializer::SerializeString(std::string& inOutString)
{
    if (IsReading())
        ReadString(inOutString);
    else
        WriteString(inOutString);
}

void BinarySerializer::SerializeBinaryData(void* inOutInstance, const int inSize)
{
    if (IsReading())
        ReadBinaryData(inOutInstance, inSize);
    else
        WriteBinaryData(inOutInstance, inSize);
}

void BinarySerializer::ReadType(void* outInstance, const Type* inType, const bool inIsPointer)
{
    if (inIsPointer)
        outInstance = inType->CreateInstance<void>();

    int fieldCount = 0;
    ReadCopyable(fieldCount);

    bool hasSerializer;
    ReadCopyable(hasSerializer);

    if (hasSerializer)
    {
        int fieldSize;
        ReadCopyable(fieldSize);
        if (TypeSerializer* customSerializer = TypeSerializer::GetSerializer(inType))
        {
            customSerializer->Serialize(outInstance, inType, this);
        }
        else
        {
            LOG_WARNING("Type: %s was serialized with type serializer but it does not exists anymore. Skipping", inType->GetName().c_str());
            myReadOffset += fieldSize;
        }
        return;
    }
    
    const List<Field> serializableFields = inType->GetFieldsWithMetadataRecursive("SerializeField");
    // Not all serializable fields were saved.
    if (serializableFields.size() != fieldCount)
        myWasLastTypeSerializationFullyComplete = false;
    
    for (int i = 0; i < fieldCount; ++i)
    {
        std::string fieldName;
        ReadString(fieldName);

        std::string fieldTypeName;
        ReadString(fieldTypeName);

        int fieldSize = 0;
        ReadCopyable(fieldSize);

        const Field* field = inType->FindFieldByNameRecursive(fieldName);

        if (!field || field->GetType()->GetName() != fieldTypeName)
        {
            LOG("BinarySerializer::ReadClass - Failed to find field: %s %s in class %s.", fieldName.c_str(), fieldTypeName.c_str(), inType->GetName().c_str());
            myReadOffset += fieldSize;
            myWasLastTypeSerializationFullyComplete = false;
            continue;
        }

        if (!field->HasMetadata("SerializeField"))
        {
            LOG("BinarySerializer::ReadClass - Field is no longer serializable: : %s %s in class %s.", fieldName.c_str(), fieldTypeName.c_str(), inType->GetName().c_str());
            myReadOffset += fieldSize;
            myWasLastTypeSerializationFullyComplete = false;
            continue;
        }

        const Type* fieldClass = field->GetType();
        void* fieldPtr = field->GetPointerToValue(outInstance);

        if (TypeSerializer* customSerializer = TypeSerializer::GetSerializer(fieldClass))
        {
            customSerializer->Serialize(fieldPtr, fieldClass, this);
        }
        else
        {
            SerializeTypeInternal(fieldPtr, fieldClass, field->IsPointer());
        }
    }
}

void BinarySerializer::WriteType(void* inInstance, const Type* inType, const bool inIsPointer)
{
    check(inType);
    
    const List<Field> fieldsToSerialize = inType->GetFieldsWithMetadataRecursive("SerializeField");
    int fieldCount = fieldsToSerialize.size();
    WriteCopyable(fieldCount);

    bool hasSerializer = false;
    TypeSerializer* customSerializer = TypeSerializer::GetSerializer(inType);

    if (customSerializer)
        hasSerializer = true;

    WriteCopyable(hasSerializer);

    if (hasSerializer)
    {
        std::ostringstream fieldBufferStream;
        BinarySerializer tempSerializer(fieldBufferStream);
        customSerializer->Serialize(inInstance, inType, &tempSerializer);
        std::string fieldBytes = fieldBufferStream.str();
        int fieldSize = static_cast<int>(fieldBytes.size());
        WriteCopyable(fieldSize);
        myOutputStream->write(fieldBytes.data(), fieldSize);
        return;
    }
    
    if (fieldCount == 0 && !missingTypesAlreadyWarnedAbout.Contains(inType->GetName()) && inType->GetName() != "")
    {
        missingTypesAlreadyWarnedAbout.Add(std::string(inType->GetName()));
        LOG_WARNING("BinarySerializer - %s contains 0 serializable fields. Should this type have a TypeSerializer?", inType->GetName().c_str());
    }
    
    for (const Field& field : fieldsToSerialize)
    {
        std::string fieldName = field.GetName();
        WriteString(fieldName);

        const Type* fieldClass = field.GetType();
        std::string fieldTypeName = fieldClass->GetName();
        WriteString(fieldTypeName);

        std::ostringstream fieldBufferStream;
        BinarySerializer tempSerializer(fieldBufferStream);

        void* fieldPtr = field.GetPointerToValue(inInstance);

        if (TypeSerializer* customSerializer = TypeSerializer::GetSerializer(fieldClass))
        {
            customSerializer->Serialize(fieldPtr, fieldClass, &tempSerializer);
        }
        else
        {
            tempSerializer.SerializeTypeInternal(fieldPtr, fieldClass, field.IsPointer());
        }

        std::string fieldBytes = fieldBufferStream.str();
        int fieldSize = static_cast<int>(fieldBytes.size());
        WriteCopyable(fieldSize);
        myOutputStream->write(fieldBytes.data(), fieldSize);
    }
}

void BinarySerializer::WriteBinaryData(void* inInstance, const int inSize)
{
    myOutputStream->write((char*)inInstance, inSize);
}

void BinarySerializer::ReadBinaryData(void* outInstance, const int inSize)
{
    memcpy(outInstance, myInBuffer.data() + myReadOffset, inSize);
    myReadOffset += inSize;
}

void BinarySerializer::WriteString(const std::string& inString)
{
    size_t length = inString.length();
    WriteCopyable(length);
    char* c = (char*)inString.data();
    myOutputStream->write(c, length * sizeof(char));
}

void BinarySerializer::ReadString(std::string& outString)
{
    size_t length;
    ReadCopyable(length);
    char* chars = (char*)(myInBuffer.data() + myReadOffset);
    outString = std::string(chars, length);
    myReadOffset += sizeof(char) * length;
}
