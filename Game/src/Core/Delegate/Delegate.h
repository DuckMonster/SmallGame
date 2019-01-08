#pragma once
template<typename InReturn, typename... InArgs>
class Delegate
{
	/* Base class for a delegate instance */
	class Instance
	{
	public:
		virtual InReturn Call(InArgs... args) = 0;
		virtual Instance* Copy() = 0;
	};

	/* Instance pointing to a raw c-style or static function */
	class RawInstance : public Instance
	{
	public:
		typedef InReturn(*FuncPtr)(InArgs...);
		RawInstance(FuncPtr ptr) : ptr(ptr)
		{}

		InReturn Call(InArgs... args) override
		{
			return ptr(args...);
		}

		Instance* Copy() override
		{
			return new RawInstance(*this);
		}

	private:
		FuncPtr ptr;
	};

	/* Instance pointing to a member function on an object */
	template<typename InClass>
	class ObjectInstance : public Instance
	{
	public:
		typedef InReturn(InClass::*FuncPtr)(InArgs...);
		ObjectInstance(InClass* object, FuncPtr func)
			:object(object), func(func)
		{}

		InReturn Call(InArgs... args) override
		{
			return (object->*func)(args...);
		}

		Instance* Copy() override
		{
			return new ObjectInstance(*this);
		}

	private:
		InClass* object;
		FuncPtr func;
	};

	/* Instance pointing to a functor (most commonly a lambda) */
	template<typename InFunctor>
	class FunctorInstance : public Instance
	{
	public:
		FunctorInstance(const InFunctor& functor)
			:functor(functor)
		{}

		InReturn Call(InArgs... args) override
		{
			return functor(args...);
		}

		Instance* Copy() override
		{
			return new FunctorInstance(*this);
		}

	private:
		InFunctor functor;
	};

public:
	static Delegate Raw(typename Delegate::RawInstance::FuncPtr func)
	{
		Delegate result;
		result.BindRaw(func);
		return result;
	}
	template<typename InObject>
	static Delegate Object(InObject* object, typename Delegate::ObjectInstance<InObject>::FuncPtr func)
	{
		Delegate result;
		result.BindObject(object, func);
		return result;
	}
	template<typename InFunctor>
	static Delegate Lambda(const InFunctor& functor)
	{
		Delegate result;
		result.BindLambda(functor);
		return result;
	}

public:
	Delegate() {}
	Delegate(const Delegate& other)
	{
		Copy(other);
	}
	Delegate(Delegate&& other)
	{
		Move(std::move(other));
	}
	~Delegate()
	{
		Clear();
	}

	// Call this delegate (if it is bound)
	InReturn Call(const InArgs&... args)
	{
		if (instance == nullptr)
			return InReturn();

		return instance->Call(args...);
	}

	// Bind a raw function (c-style or static function)
	void BindRaw(typename RawInstance::FuncPtr func)
	{
		Clear();
		instance = new RawInstance(func);
	}

	// Bind a member function on a specific object
	template<typename InObject>
	void BindObject(InObject* object, typename ObjectInstance<InObject>::FuncPtr func)
	{
		Clear();
		instance = new ObjectInstance<InObject>(object, func);
	}

	// Bind a lambda (or other functor)
	template<typename InFunctor>
	void BindLambda(const InFunctor& functor)
	{
		Clear();
		instance = new FunctorInstance<InFunctor>(functor);
	}

	// Clear this delegate, so its bound to nothing
	void Clear()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	// Returns if this delegate is bound to any receiver
	bool IsBound()
	{
		return instance != nullptr;
	}

	Delegate& operator=(const Delegate& other) { Copy(other); return *this; }
	Delegate& operator=(Delegate&& other) { Move(other); return *this; }
	InReturn operator()(InArgs... args) { return Call(args...); }

private:
	void Copy(const Delegate& other)
	{
		if (other.instance != nullptr)
			instance = other.instance->Copy();
	}
	void Move(Delegate&& other)
	{
		Clear();
		instance = other.instance;
		other.instance = nullptr;
	}

	Instance* instance = nullptr;
};