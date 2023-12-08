#pragma once

template <class T>
inline void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}


/*
template <>
struct std::hash<Key>
{
  std::size_t operator()(const Key& k) const
  {
		std::size_t res = 0;
		hash_combine(res, s.myAlbedoPath);
		hash_combine(res, s.myNormalPath);
		hash_combine(res, s.myMaterialPath);
		return res;
  }
};
*/