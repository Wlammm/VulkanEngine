#pragma once

template<typename T>
struct IsCopyable
{
	static constexpr bool value = false;
};

// This is an example. int is already copyable from std::is_trivially_copyable and will copy even if its not declared in here.
template<>
struct IsCopyable<int>
{
	static constexpr bool value = true;
};