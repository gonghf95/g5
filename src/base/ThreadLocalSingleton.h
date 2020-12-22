#ifndef BASE_THREADLOCALSINGLETON_H
#define BASE_THREADLOCALSINGLETON_H

namespace base
{

template<typename T>
class ThreadLocalSingleton : noncopyable
{
public:
	ThreadLocalSingleton() = delete;
	~ThreadLocalSingleton() = delete;

	static T& instance()
	{
		if(!t_value_)
		{
			t_value_ = new T();
			deleter_.set(t_value_);
		}
		return *t_value_;
	}

	static T* pointer()
	{
		return t_value_;
	}

private:
	static void destructor(T* obj)
	{
		assert(obj == t_value_);
		typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
		T_must_be_complete_type dummy; (void)dummy;
		delete t_value_;
		t_value_ = 0;
	}

	class Deleter
	{
	public:
		Deleter()
		{
			pthread_key_create(&pkey_, destructor);
		}

		~Deleter()
		{
			pthread_key_delete(pkey_);
		}

		void set(T* newObj)
		{
			assert(pthread_getspecific(pkey_) == NULL);
			pthread_setspecific(pkey_, newObj);
		}

		pthread_key_t pkey_;
	};

	static __thread T* t_value_;
	static Deleter deleter_;
};



} // namespace base

#endif
