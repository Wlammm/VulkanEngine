#pragma once
#include "Engine/Reflection/Class.h"
#include "TypeSerializers/TypeSerializer.h"

class Class;

class BinaryWriter
{
public:
	BinaryWriter(const std::filesystem::path& inPath);

	void Save();

	void WriteClass(void* inInstance, const Class* inClass)
	{
		Write(inClass->GetName());

		for (const Field& field : inClass->GetFieldsWithMetadata("SerializeField"))
		{
			WriteField(field.GetPointerToValue(inInstance), field);	
		}
	}

	void WriteField(void* inInstance, const Field& inField)
	{
		Write(inField.GetName());
		
		const Class* fieldType = inField.GetType();
		Write(fieldType->GetFullName());

		if (ITypeSerializer* serializer = ITypeSerializer::GetSerializer(fieldType))
			serializer->Write(inInstance, this);
		else if (fieldType->IsCopyable())
			myStream.write((char*)inInstance, fieldType->GetSize());
		else
			WriteClass(inInstance, fieldType);
	}
	
	void Write(const std::string& inString)
	{
		size_t length = inString.length();
		Write(length);
		char* c = (char*)inString.data();
		myStream.write(c, length * sizeof(char));
	}

	void Write(const std::filesystem::path& inPath)
	{
		std::wstring data = inPath.wstring();
		size_t dataSize = data.length();
		Write(dataSize);

		char* c = (char*)data.c_str();
		myStream.write(c, dataSize * sizeof(wchar_t));
	}

	template<typename T>
	void Write(const List<T>& inList)
	{
		Write(inList.size());
		char* c = (char*)inList.data();
		myStream.write(c, sizeof(T) * inList.size());
	}

	template<typename T>
	void Write(const T& inValue)
	{
		char* c = (char*)&inValue;
		size_t size = sizeof(T);
		myStream.write(c, size);
	}

	void Write(void* inInstance, const int inSize)
	{
		myStream.write((char*)inInstance, inSize);
	}

private:
	std::ofstream myStream;
};
