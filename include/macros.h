// macros.h : defines
//

#ifndef _MACROS_H_HUXL_
#define _MACROS_H_HUXL_

#define MEMBER_VARIABLE_GET(_Result, _Fun, _mVal) \
	_Result _Fun() const {return (_mVal);}

#define MEMBER_VARIABLE_SET(_Result, _Fun, _mVal) \
	_Result _Fun() {return (_mVal);}

#define MEMBER_VARIABLE_REF MEMBER_VARIABLE_SET
//#define MEMBER_VARIABLE_REF(_Result, _Fun, _mVal) \
//	_Result _Fun() {return (_mVal);}

#define MEMBER_VARIABLE_GET_SET(_Result, _Fun, _mVal) \
	_Result& _Fun() {return (_mVal);} \
	_Result const& _Fun() const {return (_mVal);}

#define MEMBER_VARIABLE_INDEXER_GET_SET_TEMPLATE(_Result, _mVal) \
	template<typename _IdxType> _Result& operator [](_IdxType idx) {return _mVal[idx];} \
	template<typename _IdxType> _Result const& operator [](_IdxType idx) const{return _mVal[idx];}

#define MEMBER_VARIABLE_INDEXER_GET_TEMPLATE(_Result, _mVal) \
	template<typename _IdxType> _Result operator [](_IdxType idx) const{return _mVal[idx];}

#define MEMBER_VARIABLE_GET_SET_INDEXER(_Result, _IdxType, _mVal) \
	_Result& operator [](_IdxType idx) {return _mVal[idx];} \
	_Result const& operator [](_IdxType idx) const{return _mVal[idx];}

#define MEMBER_VARIABLE_GET_INDEXER(_Result, _IdxType, _mVal) \
	_Result operator [](_IdxType idx) {return _mVal[idx];}

#define MEMBER_VARIABLE_INDEXER_TEMPLATE MEMBER_VARIABLE_INDEXER_GET_SET_TEMPLATE
#define MEMBER_VARIABLE_INDEXER MEMBER_VARIABLE_GET_SET_INDEXER

#define NAMESPACE_BEGIN(NameSpace_Name) namespace NameSpace_Name{
#define NAMESPACE_END(NameSpace_Name) }

#define NON_COPYABLE_OBJECT(ClassName) \
	private: \
	ClassName(const ClassName&); \
	ClassName& operator=(const ClassName&);


#define OVERWRITE_FUNCTION_1(function, _Arg, _ArgName) \
	_Myt& function(_Arg _ArgName) \
	{	\
		base().function(_ArgName); \
		return *this; \
	}

#define OVERWRITE_FUNCTION_2(function, _Arg1, _ArgName1, _Arg2, _ArgName2) \
	_Myt& function(_Arg1 _ArgName1, _Arg2 _ArgName2) \
	{	\
		base().function(_ArgName1, _ArgName2); \
		return *this; \
	}

#define OVERWRITE_FUNCTION_3(function, _Arg1, _ArgName1, _Arg2, _ArgName2, _Arg3, _ArgName3) \
	_Myt& function(_Arg1 _ArgName1, _Arg2 _ArgName2, _Arg3 _ArgName3) \
	{	\
		base().function(_ArgName1, _ArgName2, _ArgName3); \
		return *this; \
	}

#define OVERWRITE_FUNCTION_4(function, _Arg1, _ArgName1, _Arg2, _ArgName2, _Arg3, _ArgName3, Arg4, _ArgName4) \
	_Myt& function(_Arg1 _ArgName1, _Arg2 _ArgName2, _Arg3 _ArgName3, Arg4 _ArgName4) \
	{	\
		base().function(_ArgName1, _ArgName2, _ArgName3, _ArgName4); \
		return *this; \
	}

#define CATCH_EXCEPTIONS_COUT \
	catch(sc::scerror& sce){ \
		std::cerr<<"["<<typeinfo_name(typeid(sce).name())<<"]"<<sce.what()<<"\n" \
				 <<sce.trace_message()<<std::endl; \
	}	\
	catch(std::exception& ex){ \
		std::cerr<<"["<<typeinfo_name(typeid(ex).name())<<"]"<<ex.what()<<std::endl; \
	}catch(...){ \
		std::cerr<<"unknown exception!"<<std::endl; \
	}
#endif
