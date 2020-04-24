#pragma once

template <typename K, typename V1, typename V2>
class Entry
{
public:
	Entry(const K& k = K(), const V1& v = V1(), const V2& v2 = V2()) : _key(k), _value1(v), _value2(v2) {}
	const K& key() const { return _key; }
	const V1& value1() const { return _value1; }
	const V2& value2() const { return _value2; }
	void setKey(const K& k) { _key = k; }
	void setValue1(const V1& v) { _value1 = v; }
	void setValue2(const V2& v2) { _value2 = v2; }
private:
	K _key;
	V1 _value1;
	V2 _value2;
};
