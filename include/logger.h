// logger.h: log file
//

#ifndef _LOG_H_HUXL_
#define _LOG_H_HUXL_

#include <iostream>
#include <fstream>
#include <memory>

#include <macros.h>
#include <singleton.h>
#include <stringX.h>

NAMESPACE_BEGIN(kog)

class logger
{
	NON_COPYABLE_OBJECT(logger);
public:
	logger()
	: os_(&std::clog)
	{
	}

	logger(const std::string& log_file)
		: os_(new std::ofstream(log_file.c_str()))
		, need_clear_(true)
	{
		if (!((std::ofstream*)os_)->is_open()) 
		{
			delete os_;
			os_ = NULL;
			need_clear_ = false;
			throw std::runtime_error("can't open logger file!");
		}
	}

	~logger()
	{
		clear();
	}

public:
	template<typename _Tx>
	logger& operator<<(const _Tx& v)
	{
		if(!os_) throw std::runtime_error("empty log stream!");
		(*os_)<<v;
		return *this;
	}
public:
	void reset_log_ostream(std::ostream& os)
	{
		clear();
		os_ = &os;
		need_clear_ = false;
	}

	void reset_log_file(const std::string& log_file)
	{
		clear();
		std::auto_ptr<std::ofstream> tmp(new std::ofstream(log_file.c_str()));
		if(!tmp->is_open()) throw std::runtime_error("can't open logger file!");
		os_ = tmp.release();
		need_clear_ = true;
	}

	std::ostream& getos() const
	{
		if(!os_) throw std::runtime_error("empty log stream!");
		return *os_;
	}
private:
	void clear()
	{
		if(need_clear_ && os_)
		{
			delete os_;
			os_ = NULL;
			need_clear_ = false;
		}
	}
private:
	std::ostream* os_;
	bool need_clear_;
};

class loggermanager : public singleton<loggermanager>
{
public:
	loggermanager()
		: lf_("rlog.txt")
	{
	}

	~loggermanager()
	{
	}
public:
	loggermanager& dump(const std::string& content)
	{
		lf_<<content;
        lf_.getos()<<std::endl;
		return *this;
	}

	logger& get_logger()
	{
		return lf_;
	}
private:
	logger lf_;
};

#ifdef VISUAL_STDIO
#define logstring(fmt, ...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][message]", __FILE__, __LINE__) + stringX::format(fmt, __VA_ARGS__))
#define logwarning(fmt, ...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][warning]", __FILE__, __LINE__) + stringX::format(fmt, __VA_ARGS__))
#define logerror(fmt, ...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][error]", __FILE__, __LINE__) + stringX::format(fmt, __VA_ARGS__))
#else
#define logstring(fmt, arg...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][message]", __FILE__, __LINE__) + stringX::format(fmt, ##arg))
#define logwarning(fmt, arg...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][warning]", __FILE__, __LINE__) + stringX::format(fmt, ##arg))
#define logerror(fmt, arg...) \
	kog::loggermanager::instance().dump(stringX::format("[%s:%d][error]", __FILE__, __LINE__) + stringX::format(fmt, ##arg))
#endif

#define logvalue(value) \
	kog::loggermanager::instance().get_logger()<<value

NAMESPACE_END(kog)
#endif
