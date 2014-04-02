//InitializedPod.h by Kostya Kozachuck as neurocod
#pragma once

template<class T, T defaultValue>
class InitializedPod {
	protected:
		T d;
	public:
		InitializedPod() { d = defaultValue; }
		InitializedPod(const T & t): d(t) {}
		operator T()const { return d; }
		T operator=(const T&t) { return d=t; }
};