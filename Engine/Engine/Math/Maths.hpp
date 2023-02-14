#pragma once
#include <assert.h>
#include "Matrix4x4.hpp"

template <typename T>
T DegreesToRadians(T someDegrees);

template <typename T>
T RadiansToDegrees(T someRadians);

template<typename T>
T DegreesToRadians(T someDegrees)
{
	return someDegrees * (static_cast<T>(PI) / static_cast<T>(180.0));
}
template<typename T>
T RadiansToDegrees(T someRadians)
{
	return someRadians * (static_cast<T>(180.0) / static_cast<T>(PI));
}

template<typename T>
T Max(const T& aFirst, const T& aSecond);

template<typename T>
T Min(const T& aFirst, const T& aSecond);

template<typename T>
T Abs(const T& aFirst);

template<typename T>
T Clamp(const T& aMin, const T& aMax, const T& someValue);

template<typename T>
T Lerp(T aFirst, const T& aSecond, const float& someTime);

template<typename T>
Matrix4x4<T> LerpMatrix(const Matrix4x4<T>& aFirst, const Matrix4x4<T>& aSecond, const float& someTime);

template<typename T>
void Swap(T& aFirst, T& aSecond);

template<typename T>
void IncreaseCircular(T& aValue, T aMin, T aMax);

template<typename T>
void DecreaseCircular(T& aValue, T aMin, T aMax);

// Definitions.
template<typename T>
T Max(const T& aFirst, const T& aSecond)
{
	return aFirst > aSecond ? aFirst : aSecond;
}

template<typename T>
T Min(const T& aFirst, const T& aSecond)
{
	return aFirst < aSecond ? aFirst : aSecond;
}

template<typename T>
T Abs(const T& aFirst)
{
	return aFirst > 0 ? aFirst : -aFirst;
}

template<typename T>
T Clamp(const T& aMin, const T& aMax, const T& someValue)
{
	if (aMin > aMax)
	{
		return someValue < aMax ? aMin : someValue > aMin ? aMax : someValue;
	}

	return someValue < aMin ? aMin : someValue > aMax ? aMax : someValue;
}

template<typename T>
T Lerp(T aFirst, const T& aSecond, const float& someTime)
{
	return (1.f - someTime) * aFirst + someTime * aSecond;
}

template<typename T>
Matrix4x4<T> LerpMatrix(const Matrix4x4<T>& aFirst, const Matrix4x4<T>& aSecond, const float& someTime)
{
	Matrix4x4<T> first = aFirst.Transposed();
	Matrix4x4<T> second = aSecond.Transposed();

	Vector3<T> firstPos = first.GetTranslation();
	Vector3<T> secondPos = second.GetTranslation();

	Vector3<T> firstScale = first.GetScale();
	Vector3<T> secondScale = second.GetScale();

	QuaternionT<T> firstRot = first.GetRotation();
	QuaternionT<T> secondRot = second.GetRotation();

	Vector3<T> lerpedPos = Lerp(firstPos, secondPos, someTime);
	Vector3<T> lerpedScale = Lerp(firstScale, secondScale, someTime);
	QuaternionT<T> lerpedRot = QuaternionT<T>::Slerp(firstRot, secondRot, someTime);

	Matrix4x4<T> returnMat = Matrix4x4<T>::TRS(lerpedPos, lerpedRot, lerpedScale);
	return returnMat.Transposed();
}

template<typename T>
Matrix4x4<T> LerpMatrixUntransposed(const Matrix4x4<T>& aFirst, const Matrix4x4<T>& aSecond, const float& someTime)
{
	Vector3<T> firstPos = aFirst.GetTranslation();
	Vector3<T> secondPos = aSecond.GetTranslation();

	Vector3<T> firstScale = aFirst.GetScale();
	Vector3<T> secondScale = aSecond.GetScale();

	QuaternionT<T> firstRot = aFirst.GetRotation();
	QuaternionT<T> secondRot = aSecond.GetRotation();

	Vector3<T> lerpedPos = Lerp(firstPos, secondPos, someTime);
	Vector3<T> lerpedScale = Lerp(firstScale, secondScale, someTime);
	QuaternionT<T> lerpedRot = QuaternionT<T>::Slerp(firstRot, secondRot, someTime);

	Matrix4x4<T> returnMat = Matrix4x4<T>::TRS(lerpedPos, lerpedRot, lerpedScale);
	return returnMat;
}

template<typename T>
void Swap(T& aFirst, T& aSecond)
{
	T first = aFirst;

	aFirst = aSecond;
	aSecond = first;
}

template<typename T>
void IncreaseCircular(T& aValue, T aMin, T aMax)
{
	assert(aValue >= aMin && aValue <= aMax && "Value is out of range");
	if (aValue == aMax)
	{
		aValue = aMin;
	}
	else
	{
		++aValue;
	}
}

template<typename T>
void DecreaseCircular(T& aValue, T aMin, T aMax)
{
	assert(aValue >= aMin && aValue <= aMax && "Value is out of range");
	if (aValue == aMin)
	{
		aValue = aMax;
	}
	else
	{
		--aValue;
	}
}

template<typename T>
bool AreEqual(T a, T b, T aTolerance)
{
	return Abs(a - b) <= aTolerance;
}

template<typename T>
T SmoothDamp(const T aCurrent, T aTarget, T& aOutCurrentVelocity, float aSmoothTime, const float aMaxSpeed, const float aDeltaTime)
{
	aSmoothTime = Max<T>(0.0001f, aSmoothTime);

	float num = 2.f / aSmoothTime;
	float num2 = num * aDeltaTime;
	float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
	float num4 = aCurrent - aTarget;
	float num5 = aTarget;
	float num6 = aMaxSpeed * aSmoothTime;

	num4 = Clamp<T>(-num6, num6, num4);
	aTarget = aCurrent - num4;
	float num7 = (aOutCurrentVelocity + num * num4) * aDeltaTime;
	aOutCurrentVelocity = (aOutCurrentVelocity - num * num7) * num3;
	float num8 = aTarget + (num4 + num7) * num3;
	if (num5 - aCurrent > 0.f == num8 > num5)
	{
		num8 = num5;
		aOutCurrentVelocity = (num8 - num5) / aDeltaTime;
	}

	return num8;
}