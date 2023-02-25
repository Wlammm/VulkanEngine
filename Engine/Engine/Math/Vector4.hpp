#pragma once

#include "Core/EngineDefines.hpp"

template <class T>
class Vector4
{
public:
	union
	{
		T x;
		T r;
	};
	union
	{
		T y;
		T g;
	};
	union
	{
		T z;
		T b;
	};
	union
	{
		T w;
		T a;
	};

	Vector4<T>() : x(0), y(0), z(0), w(0) {}
	Vector4<T>(const T& aX, const T& aY, const T& aZ, const T& aW) : x(aX), y(aY), z(aZ), w(aW) {}
	Vector4<T>(const Vector3<T>& aVector, const T& aW)
	{
		x = aVector.x;
		y = aVector.y;
		z = aVector.z;
		w = aW;
	}

	Vector4<T>(const Vector4<T>& aVector) = default;
	Vector4<T>& operator=(const Vector4<T>& aVector4) = default;

	inline bool operator==(const Vector4<T>& anOther) const
	{
		return x == anOther.x && y == anOther.y && z == anOther.z && w == anOther.w;
	}

	~Vector4<T>() = default;

	Vector4<T> Zero() const
	{
		return Vector4<T>();
	}

	T LengthSqr() const
	{
		return x * x + y * y + z * z + w * w;
	}

	T Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	Vector3<T> ToVec3() const
	{
		return Vector3<T>{ x / w, y / w, z / w };
	}

	Vector3<T> ToVec3IgnoreW() const 
	{
		return Vector3<T> { x, y, z };
	}

	Vector4<T> GetNormalized() const
	{
		T length = Length();
		if (length == 0)
		{
			return *this;
		}

		return *this / length;
	}

	void Normalize()
	{
		*this = GetNormalized();
	}

	T Dot(const Vector4<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z + w * aVector.w;
	}

	template<typename Temp>
	Vector4<Temp> Cast() const
	{
		return { (Temp)x, (Temp)y, (Temp)z, (Temp)w };
	}

	T& operator[] (const int aIndex)
	{
		switch (aIndex)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			check("index out of range");
		}

		return x;
	}

	const T& operator[](const int aIndex) const
	{
		switch (aIndex)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			check("index out of range");
		}

		return x;
	}
};

template <class T>
Vector4<T> operator+(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
{
	return Vector4<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z, aVector0.w + aVector1.w);
}

template <class T>
Vector4<T> operator-(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
{
	return Vector4<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z, aVector0.w - aVector1.w);
}

template <class T>
Vector4<T> operator*(const Vector4<T>& aVector0, const Vector4<T>& aVector1)
{
	return Vector4<T>(aVector0.x * aVector1.x, aVector0.y * aVector1.y, aVector0.z * aVector1.z, aVector0.w * aVector1.w);
}

template <class T>
Vector4<T> operator*(const Vector4<T>& aVector, const T& aScalar)
{
	return Vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
}

template <class T>
Vector4<T> operator*(const T& aScalar, const Vector4<T>& aVector)
{
	return aVector * aScalar;
}

template <class T>
Vector4<T> operator/(const Vector4<T>& aVector, const T& aScalar)
{
	check(aScalar != 0 && "Cannot divide by zero");
	if (aScalar == static_cast<T>(0))
	{
		return aVector;
	}

	T multiplier = static_cast<T>(1) / aScalar;
	return Vector4<T>(aVector.x * multiplier, aVector.y * multiplier, aVector.z * multiplier, aVector.w * multiplier);
}

template <class T>
void operator+=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
{
	aVector0 = aVector0 + aVector1;
}

template <class T>
void operator-=(Vector4<T>& aVector0, const Vector4<T>& aVector1)
{
	aVector0 = aVector0 - aVector1;
}

template <class T>
void operator*=(Vector4<T>& aVector, const T& aScalar)
{
	aVector = aVector * aScalar;
}

template <class T>
void operator/=(Vector4<T>& aVector, const T& aScalar)
{
	aVector = aVector / aScalar;
}

using Color = Vector4<float>;
using Vec4f = Vector4<float>;
using Vec4i = Vector4<int>;
using Vec4ui = Vector4<unsigned>;