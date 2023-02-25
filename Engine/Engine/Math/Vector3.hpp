#pragma once

#include "Core/EngineDefines.hpp"

template<class T>
class Vector4;

template <class T>
class Vector3
{
public:
	T x;
	T y;
	T z;

	Vector3<T>() : x(0), y(0), z(0) {}
	Vector3<T>(const T& inScalar) : x(inScalar), y(inScalar), z(inScalar) {}
	Vector3<T>(const T& aX, const T& aY, const T& aZ) : x(aX), y(aY), z(aZ) {}

	Vector3<T>(const Vector3<T>& aVector) = default;
	Vector3<T>& operator=(const Vector3<T>& aVector3) = default;
	~Vector3<T>() = default;

	static Vector3<T> Zero() { return Vector3<T>(0, 0, 0); }
	static Vector3<T> One() { return Vector3<T>(1, 1, 1); }
	static Vector3<T> Up() { return Vector3<T>(0, 1, 0); }
	static Vector3<T> Forward() { return Vector3<T>(0, 0, 1); }
	static Vector3<T> Right() { return Vector3<T>(1, 0, 0); }

	T& operator[](const int aIndex)
	{
		switch (aIndex)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
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
		default:
			check("index out of range");
		}

		return x;
	}

	T LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	Vector4<T> ToVec4(const T& inW) const
	{
		return Vector4<T>{ x, y, z, inW }; 
	}

	T Length() const
	{
		return static_cast<T>(sqrt(LengthSqr()));
	}

	Vector3<T> GetNormalized() const
	{
		T length = Length();
		if (length == 0)
			return *this;

		return *this / length;
	}

	void Normalize()
	{
		*this = GetNormalized();
	}

	T Dot(const Vector3<T>& aVector) const
	{
		return x * aVector.x + y * aVector.y + z * aVector.z;
	}

	Vector3<T> Cross(const Vector3<T>& aVector) const
	{
		return Vector3<T>(y * aVector.z - z * aVector.y, z * aVector.x - x * aVector.z, x * aVector.y - y * aVector.x);
	}

	Vector3<T> operator-() const
	{
		return *this * -1;
	}

	Vector3<T> Reflect(const Vector3<T>& aNormal) const
	{
		Vector3<T> normal = aNormal.GetNormalized();
		return *this - normal * normal.Dot(*this) * static_cast<T>(2);
	}

	template<typename Temp>
	Vector3<Temp> Cast() const
	{
		return { static_cast<Temp>(x), static_cast<Temp>(y), static_cast<Temp>(z) };
	}

	static Vector3<T> Lerp(const Vector3<T>& aStart, const Vector3<T>& aEnd, const float aT)
	{
		return (1 - aT) * aStart + aT * aEnd;
	}
};

template <class T>
Vector3<T> operator+(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
{
	return Vector3<T>(aVector0.x + aVector1.x, aVector0.y + aVector1.y, aVector0.z + aVector1.z);
}

template <class T>
Vector3<T> operator-(const Vector3<T>& aVector0, const Vector3<T>& aVector1)
{
	return Vector3<T>(aVector0.x - aVector1.x, aVector0.y - aVector1.y, aVector0.z - aVector1.z);
}

template <class T>
Vector3<T> operator*(const Vector3<T>& aVector, const T& aScalar)
{
	return Vector3<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar);
}

template <class T>
Vector3<T> operator*(const T& aScalar, const Vector3<T>& aVector)
{
	return aVector * aScalar;
}

template <class T>
Vector3<T> operator/(const Vector3<T>& aVector, const T& aScalar)
{
	check(aScalar != 0 && "Cannot divide by zero");
	if (aScalar == static_cast<T>(0))
	{
		return aVector;
	}

	T multiplier = static_cast<T>(1) / aScalar;
	return Vector3<T>(aVector.x * multiplier, aVector.y * multiplier, aVector.z * multiplier);
}

template <class T>
Vector3<T> operator/(const T& aScalar, const Vector3<T>& aVector)
{
	check(aScalar != 0 && "Cannot divide by zero");
	if (aScalar == static_cast<T>(0))
		return aVector;

	return Vector3<T>(aScalar / aVector.x, aScalar / aVector.y, aScalar / aVector.z);
}


template <class T>
void operator+=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
{
	aVector0 = aVector0 + aVector1;
}

template <class T>
void operator-=(Vector3<T>& aVector0, const Vector3<T>& aVector1)
{
	aVector0 = aVector0 - aVector1;
}

template <class T>
void operator*=(Vector3<T>& aVector, const T& aScalar)
{
	aVector = aVector * aScalar;
}

template <class T>
void operator/=(Vector3<T>& aVector, const T& aScalar)
{
	aVector = aVector / aScalar;
}

template <class T>
inline Vector3<T> operator*(const Vector3<T> aVector, const Vector3<T> anotherVector)
{
	return Vector3<T>{aVector.x* anotherVector.x, aVector.y* anotherVector.y, aVector.z* anotherVector.z};
}

template <class T>
inline void operator*=(Vector3<T> aVector, const Vector3<T> anotherVector)
{
	aVector = aVector * anotherVector;
}

template <class T>
inline Vector3<T> operator/(const Vector3<T> aVector, const Vector3<T> anotherVector)
{
	return Vector3<T>{aVector.x / anotherVector.x, aVector.y / anotherVector.y, aVector.z / anotherVector.z};
}

template <class T>
inline void operator/=(Vector3<T> aVector, const Vector3<T> anotherVector)
{
	aVector = aVector / anotherVector;
}

template <class T>
inline bool operator==(const Vector3<T>& aVector3_1, const Vector3<T>& aVector3_2)
{
	return (aVector3_1.x == aVector3_2.x &&
		aVector3_1.y == aVector3_2.y &&
		aVector3_1.z == aVector3_2.z);
}

template <class T>
inline bool operator!=(const Vector3<T>& aVector3_1, const Vector3<T>& aVector3_2)
{
	return (aVector3_1.x != aVector3_2.x ||
		aVector3_1.y != aVector3_2.y ||
		aVector3_1.z != aVector3_2.z);
}

template<class T>
inline Vector3<T> operator-(const Vector3<T>& aVector)
{
	return { -aVector.x, -aVector.y, -aVector.z };
}

using Vec3f = Vector3<float>;
using Vec3i = Vector3<int>;
using Vec3ui = Vector3<unsigned>;