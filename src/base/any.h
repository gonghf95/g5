#ifndef BASE_ANY_H
#define BASE_ANY_H

#include <assert.h>
#include <typeinfo>

namespace base
{

class any
{
public:

public:
	any() : content(0)
	{
	}

	template<typename ValueType>
	any(const ValueType& value) 
		: content(new holder<ValueType>(value))
	{
	}

	any(const any& other)
		: content(other.content ? other.content->clone() : 0)
	{
	}

	~any()
	{
		delete content;
	}

public: // modifiers

	any& swap(any& rhs)
	{
		placeholder* tmp = content;
		content = rhs.content;
		rhs.content = tmp;
		return *this;
	}

	template<typename ValueType>
	any& operator=(const ValueType& rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}

	any& operator=(const any& rhs)
	{
		any(rhs).swap(*this);
		return *this;
	}

public: // queries
	bool empty() const
	{
		return !content;
	}

	void clear()
	{
		any().swap(*this);
	}

	const std::type_info& type() const
	{
		return content ? content->type() : typeid(void);
	}

public: // types

	class placeholder
	{
	public: // structors
		virtual ~placeholer() { }

	public: // queries
		virtual const std::typeinfo& type() const = 0;
		virtual placeholder* clone() const = 0;
	};

	template<typename ValueType>
	class holder : public placeholder
	{
	public: // structors
		holder(const ValueType& value) : held(value) { }

	public: // queries
		virtual const std::type_info& type() const
		{ 
			return typeid(ValueType); 
		}

		virtual placeholder* clone() const
		{ 
			return new holder(held); 
		}

	public: // representation

		ValueType held;
	};

private:
	template<typename ValueType>
	friend ValueType any_cast(const any& operand);

	placeholer* content;

};

template<typename ValueType>
ValueType any_cast(const any& operand)
{
	assert(operand.type() == typeid(ValueType));
	return static_cast<any::holder<ValueType>*>(operand.content)->held;
}

} // namespace

#endif
