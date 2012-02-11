// markuputils.h : markup useful functions and xmlfile wrapper
//

#ifndef _MARKUP_UTILS_H_SC_
#define _MARKUP_UTILS_H_SC_

#include <macros.h>
#include <xml/Markup.h>
#include <memory>

NAMESPACE_BEGIN(xml)

NAMESPACE_BEGIN(utils)

class file
{
	typedef std::fstream fstream;
public:
	file(const std::string& fname, bool inORout)
		: fname_(fname)
		, inORout_(inORout)
		, fs_(new CMarkup)
	{
		if(inORout_ == in)
		{
			fs_->Load(fname_.c_str());
		}
	}

	virtual ~file()
	{
		if(inORout_ == out)
		{
			fs_->Save(fname_.c_str());
		}
	}
public:
	void into()
	{
		fs_->IntoElem();
	}

	void outof()
	{
		fs_->OutofElem();
	}
public:
	void swap(file& other)
	{
		fs_.swap(other.fs_);
		fname_.swap(other.fname_);
		std::swap(inORout_, other.inORout_);
	}
protected:
	std::auto_ptr<CMarkup> fs_;
	std::string fname_;
	bool inORout_;
	enum {in = 0, out = 1};
};

NAMESPACE_END(utils)

class node
{
	friend class ifile;
private:
	node(utils::ifile* f = NULL)
		: f_(f)
	{
	}

public:
	tstring operator[](const tstring& tagname)
	{
		CMarkup& doc = *f_->fs_;
		tstring t = doc.GetAttrib(tagname);
		if(!t.empty()) return t;
		doc.IntoElem();
		while(doc.FindElem())
		{
			tstring s = doc.GetTagName();
			if(s == tagname) 
			{
				t = doc.GetData();
				break;
			}
		}
		doc.OutOfElem();
		return t;
	}
private:
	ifile* f_;
};

class utilsfunc
{
public:
};

class ifile : public virtual utils::file
{
	friend node;
public:
	ifile(const std::string& fname)
		: utils::file(fname, utils::file::in)
	{
		cnode_.f_ = this;
	}

public:
	void swap(ifile& other)
	{
		utils::file::swap(other);
	}
private:
	node cnode_;
};

NAMESPACE_END(xml)
#endif
