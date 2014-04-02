//Vector2d.h by Kostya Kozachuck as neurocod
#pragma once

template<typename T>
class Vector2d {
	QVector<T> m_v;
	QPoint m_size;
	public:
		Vector2d(): m_size(0, 0) {
		}
		virtual ~Vector2d() {}
		void clear() {
			m_v.clear();
			m_size = QPoint(0, 0);
		}
		void resize(const QSize & sz) {
			resize(sz.width(), sz.height());
		}
		void resize(const QSize & sz, T value) {
			resize(sz.width(), sz.height(), value);
		}
		void resize(int x, int y) {
			if(x<=0 || y<=0) {
				clear();
				return;
			}
			m_size = QPoint(x, y);
			m_v.resize(x*y);
		}
		void resize(int x, int y, T value) {
			if(x<=0 || y<=0) {
				clear();
				return;
			}
			m_size = QPoint(x, y);
			m_v.fill(value, x*y);
		}
		T get(int x, int y)const {
			ASSERT(x>=0 && y>=0 && x<m_size.x() && y<m_size.y());
			return m_v[y*m_size.x() + x];
		}
		void set(int x, int y, T t)const {
			ASSERT(x>=0 && y>=0 && x<m_size.x() && y<m_size.y());
			m_v[y*m_size.x() + x] = t;
		}
		T & ref(int x, int y) {
			ASSERT(x>=0 && y>=0 && x<m_size.x() && y<m_size.y());
			return m_v[y*m_size.x() + x];
		}
};