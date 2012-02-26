// global_values.h: global values
// 

#ifndef _GLOBAL_VALUES_H_HUXL_
#define _GLOBAL_VALUES_H_HUXL_

#include <string>
#include <map>
#include <typeinfo>
#include <sstream>
#include <fstream>
#include <memory>
#include <stdexcept>

#include <macros.h>
#include <singleton.h>

#define LOAD_GLOBAL_VALUES_FROMFILE 1

#define GVALUE_D(type, name, default_value) \
	(kog::gv().get(name, (default_value)).toValue<type>())
#define GVALUE(type, name) \
	(kog::gv()[name].toValue<type>())
#define GVALUE_A(type, name, value) \
	((kog::gv()[name] = (value)).toValue<type>())

NAMESPACE_BEGIN(kog)

class refobject
{
private:
	class holder
	{
	public:
		holder() {}
		#ifdef WIN32
		virtual ~holder() = 0 {};
		#else
		virtual ~holder() {};
		#endif

	public:
		virtual const std::type_info& type() const = 0;
		virtual holder& assign(const holder& V) = 0;
		virtual holder* clone() const = 0;
		virtual std::string to_string() const = 0;
		virtual holder& from_string(const std::string& str) = 0;
		//virtual bool equal(const holder& _Other) const = 0;
	};

	template<typename _Tx>
	class object : public holder
	{
	public:
		object()
		{}

		object(const _Tx& v)
			: value_(v)
		{}
	public:
		const std::type_info& type() const
		{
			return typeid(_Tx);
		}

		object<_Tx>& assign(const holder& V)
		{
			if(V.type() != type())
			{
				throw std::runtime_error(std::string("can't assign [") + V.type().name() + "] to [" + type().name() + "]");
			}
			value_ = ((const object<_Tx>&)V).value_;
			return *this;
		}

		object<_Tx>* clone() const
		{
			return new object<_Tx>(value_);
		}

		std::string to_string() const
		{
			std::ostringstream ss;
			ss<<value_;
			return ss.str();
		}

		object<_Tx>& from_string(const std::string& str)
		{
			std::istringstream ss;
			ss.str(str);
			ss>>value_;
			return *this;
		}
	public:
		const _Tx& get() const
		{
			return value_;
		}

		_Tx& get()
		{
			return value_;
		}

	private:
		_Tx value_;
	};
public:
	refobject()
	{
	}

	template<typename _Tx>
	refobject(const _Tx& v)
		: obj_(new object<_Tx>(v))
	{
	}

	refobject(const refobject& v)
		: obj_(v.obj_.get() == NULL ? NULL : v.obj_->clone())
	{}

	~refobject()
	{
	}
public:
	template<typename _Tx>
	const _Tx& toValue() const
	{
		if(obj_.get() == NULL)
			throw std::runtime_error("can't get value from an empty object");
		else if(obj_->type() != typeid(_Tx))
			throw std::runtime_error(std::string("can't get value from [") + obj_->type().name() + "] to [" + typeid(_Tx).name()+ "]");
		return ((const object<_Tx>*)(obj_.get()))->get();
	}

	template<typename _Tx>
	_Tx& toValue()
	{
		if(obj_.get() == NULL)
			throw std::runtime_error("can't get value from an empty object");
		else if(obj_->type() != typeid(_Tx))
			throw std::runtime_error(std::string("can't get value from [") + obj_->type().name() + "] to [" + typeid(_Tx).name()+ "]");
		return ((object<_Tx>*)(obj_.get()))->get();
	}
public:
	template<typename _Ty>
	refobject& operator=(const _Ty& _V)
	{
		refobject tmp(_V);
		return *this = tmp;
	}

	refobject& operator=(const refobject& _Other)
	{
		if(obj_.get() == NULL && _Other.obj_.get() == NULL)
			return *this;
		else if(_Other.obj_.get() == NULL)
			obj_.release();
		else obj_.reset(_Other.obj_->clone());
		return *this;
	}

public:
	friend std::ostream& operator<<(std::ostream& os, const refobject& ref)
	{
		if(ref.obj_.get() == NULL)
		{
			os<<"(empty object)";
		}
		else os<<ref.obj_->to_string();
		return os;
	}
private:
	std::auto_ptr<holder> obj_;
};

class global_values : public kog::singleton<global_values>
{
	NON_COPYABLE_OBJECT(global_values)
private:
#if	LOAD_GLOBAL_VALUES_FROMFILE
	void load_from_file();
	static std::string load_file_;
public:
	static std::string& setautoloadfile(const std::string& file)
	{
		return load_file_ = file;
	}
#else
	void load_from_file()
	{
	}
#endif
public:
	global_values()
	{
		load_from_file();
	}

	~global_values()
	{
		if(!logFile_.empty())
		{
			std::ofstream log_ofs(logFile_.c_str());
			//if(!log_ofs.is_open()) throw std::runtime_error("can");
			for(std::map<std::string, refobject>::const_iterator iter = values_.begin();
				iter != values_.end(); ++ iter)
			{
				log_ofs<<"["<<iter->first<<"]\t"<<iter->second<<"\n";
			}
			log_ofs.close();
		}
	}
public:
	template<typename _Tx>
	void insert(const std::string& name, const _Tx& value)
	{
		if(name.empty()) throw std::runtime_error("can't create empy name value!");
		std::map<std::string, refobject>::const_iterator iter = values_.find(name);
		if(iter != values_.end()) throw std::runtime_error("there have a value named" + name + " already!");

		(*this)[name] = refobject(value);
	}
public:
	refobject& operator[](const std::string& name)
	{
		return get(name);
	}

public:
	refobject& get(const std::string& name, const refobject& defaultV = refobject())
	{
		std::map<std::string, refobject>::iterator iter = values_.find(name);
		if(iter == values_.end())
		{
			values_.insert(std::make_pair(name, defaultV));
			iter = values_.find(name);
		}
		return iter->second;
	}

	void del(const std::string& name)
	{
		std::map<std::string, refobject>::iterator iter = values_.find(name);
		if(iter == values_.end())
		{
			throw std::runtime_error("can't find value[" + name + "]");
		}
		values_.erase(iter);
	}

	bool contain(const std::string& name) const
	{
		return values_.end() != values_.find(name);
	}
public:
	MEMBER_VARIABLE_GET_SET(std::string, LogFile, logFile_);

private:
	std::map<std::string, refobject > values_;
	std::string logFile_;
};

inline global_values& gv()
{
	return global_values::instance();
}

NAMESPACE_END(kog)

#endif
