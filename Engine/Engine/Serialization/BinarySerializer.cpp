#include "EnginePch.h"
#include "BinarySerializer.h"

BinarySerializer::BinarySerializer(const std::filesystem::path& inPath, const Mode inMode)
{
    myMode = inMode;
    myPath = inPath;

    if (IsWriting())
    {
        myIsStreamOpen = true;
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

void BinarySerializer::SerializeClass(void* inOutInstance, const Class* inClass, const bool inIsPointer)
{
    if (IsReading())
        ReadClass(inOutInstance, inClass, inIsPointer);
    else
        WriteClass(inOutInstance, inClass, inIsPointer);
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

void BinarySerializer::ReadClass(void* outInstance, const Class* inClass, const bool inIsPointer)
{
    if (inIsPointer)
        outInstance = inClass->CreateInstance<void>();

    int fieldCount = 0;
    ReadCopyable(fieldCount);

    for (int i = 0; i < fieldCount; ++i)
    {
        std::string fieldName;
        ReadString(fieldName);

        std::string fieldTypeName;
        ReadString(fieldTypeName);

        int fieldSize = 0;
        ReadCopyable(fieldSize);

        const Field* field = inClass->FindFieldByName(fieldName);

        if (!field || field->GetClass()->GetName() != fieldTypeName)
        {
            LOG("BinarySerializer::ReadClass - Failed to find field: %s %s in class %s.", fieldName.c_str(), fieldTypeName.c_str(), inClass->GetName().c_str());
            myReadOffset += fieldSize;
            continue;
        }

        if (!field->HasMetadata("SerializeField"))
        {
            LOG("BinarySerializer::ReadClass - Field is no longer serializable: : %s %s in class %s.", fieldName.c_str(), fieldTypeName.c_str(), inClass->GetName().c_str());
            myReadOffset += fieldSize;
            continue;
        }

        const Class* fieldClass = field->GetClass();
        void* fieldPtr = field->GetPointerToValue(outInstance);

        if (TypeSerializer* customSerializer = TypeSerializer::GetSerializer(fieldClass))
        {
            customSerializer->Serialize(fieldPtr, fieldClass, this);
        }
        else
        {
            SerializeClass(fieldPtr, fieldClass, field->IsPointer());
        }
    }
}

void BinarySerializer::WriteClass(void* inInstance, const Class* inClass, const bool inIsPointer)
{
    const List<Field> fieldsToSerialize = inClass->GetFieldsWithMetadata("SerializeField");
    int fieldCount = fieldsToSerialize.size();
    WriteCopyable(fieldCount);

    for (const Field& field : fieldsToSerialize)
    {
        std::string fieldName = field.GetName();
        WriteString(fieldName);

        const Class* fieldClass = field.GetClass();
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
            tempSerializer.SerializeClass(fieldPtr, fieldClass, field.IsPointer());
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
