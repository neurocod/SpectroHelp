//Hash2.h by Kostya Kozachuck as neurocod
#pragma once

template<typename T1, typename T2, class Value>
class Hash2 {
	typedef QPair<T1, T2> Key;
	QHash<Key, Value> m_d;
	public:
		Hash2();
		virtual ~Hash2() {}
		Value value(T1 t1, T2 t2) {
			return m_d.value(Key(t1, t2));
		}
		Value value(T1 t1, T2 t2, Value defaultValue) {
			return m_d.value(Key(t1, t2), defaultValue);
		}
		void set
};