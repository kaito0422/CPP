#include <iostream>
#include <string>

template <typename T> class SmartPtr;

/* 用于描述一个对象资源，一个RefPtr的存在与否应该绑定于资源的存在与否 */
template <typename T>
class RefPtr {
public:
	RefPtr() { ptr = NULL; count = 0; }
	RefPtr(T x) {
		ptr = new T(x);
		count = 1;
	}
	friend class SmartPtr<T>;
	~RefPtr() {
		count--;
		if(!count)
			delete ptr;
	}
private:
	T *ptr;     // 用于指向对象资源
	int count;  // 用于计数引用次数
};

template <typename T>
class SmartPtr {
public:
	SmartPtr() {  }
	SmartPtr(T x) {
		rp = new RefPtr<T>(x);
	}
	SmartPtr(SmartPtr<T> &x) {  // 在定义智能指针的时候，如果采用'='赋值，则也需要增加引用次数
		rp = x.rp;
		x.rp->count++;
	}
	SmartPtr<T>& operator=(SmartPtr<T> &x) {
		rp = x.rp;
		rp->count++;  // 增加引用次数
		return *this;
	}
	T& operator*() {
		return *(rp->ptr);
	}
	T* operator->() {
		return rp->ptr;
	}
	int getCount() { return rp->count; }
	~SmartPtr() {
		rp->~RefPtr();  // 一个指针释放对该资源的引用，对象资源的引用次数减一
	}
private:
	RefPtr<T> *rp;
};

int main()
{
	SmartPtr<std::string> p(std::string("kaito"));
	std::cout << *p << std::endl;
	std::cout << p.getCount() << std::endl;
	{
		SmartPtr<std::string> p1 = p;
		std::cout << p1.getCount() << std::endl;
		{
			SmartPtr<std::string> p2 = p;
			std::cout << p2.getCount() << std::endl;
			{
				SmartPtr<std::string> p3;
				p3 = p;
				std::cout << p3.getCount() << std::endl;
			}
			std::cout << p2.getCount() << std::endl;
		}
		std::cout << p1.getCount() << std::endl;
	}
	std::cout << p.getCount() << std::endl;
	


	system("pause");
	return 0;
}
