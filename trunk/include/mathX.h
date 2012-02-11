// mathX.h: 
//

#ifndef _MATHX_H_HUXL_
#define _MATHX_H_HUXL_

#include <cmath>
#include <macros.h>

NAMESPACE_BEGIN(kog)

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

class math
{
public:
	// value * value
	template<typename T>
	static T square(const T value)
	{
		return value * value;
	}

	// sqrt(value)
	template<typename T>
	static T sqrt(const T value)
	{
		return (T)(::sqrt(1.0*value));
	}

	template<typename T>
	static T log(const T value)
	{
		return (T)(::log(1.0 * value));
	}

	template<typename T>
	static T log2(const T value)
	{
		return (T)(::log(1.0*value)/::log(2.0));
	}

	template<typename T>
	static T ln(const T value)
	{
		return log(value);
	}

	template<typename T>
	static  T log10(const T value)
	{
		return (T)(::log10(value));
	}

	// the arctan of (x, y)
	// 0 .. PI*2
	template<typename T>
	static double atan(const T y, const T x)
	{
		double value = ::atan2(1.0*y, 1.0*x);
		if(y < 0)
		{
			value += PI * 2;
		}
		return value;
	}

	template<typename T>
	static T abs(const T value)
	{
		return value < 0 ? -value : value;
	}

	template<typename _Result, typename _Arg>
	static _Result round(const _Arg value)
	{
		return (_Result)(value + 0.5f);
	}

	template<typename T>
	class interpolate
	{
	public:
		// function : getIndex
		// binary search the vector,
		// if find the val in it, return the index in the vector,
		// or will return the opposite number of the index of the first element which greater than val in the vector
		static int getIndex(const T* begin, const size_t nSize, const T& val)
		{
			//if(nSize == 0) throw std::exception("error: invalid sequence!");
			int low, high, mid;
			low = 0;
			high = (int)nSize - 1;
			while(low <= high)
			{
				mid = (low + high) >> 1;
				if(begin[mid] == val) return mid;
				else if(begin[mid] < val) low = mid + 1;
				else high = mid - 1;
			}
			return -(low + 1);
		}

		static T calculateValue(const T* xBegin, const T* yBegin, const size_t nSize, int index, const T& val)
		{
			if(0 == nSize)
				throw std::exception("error: empty sequence!");

			if(index >= 0)
			{
				if(nSize > (size_t)index) return yBegin[index];
				else return yBegin[nSize-1];
			}
			else
			{
				index = ~index;
				if(index < (int)nSize)
				{
					if(index <= 0) return yBegin[0];
					else return yBegin[index-1] * (val - xBegin[index-1]) / (xBegin[index] - xBegin[index-1]) +
						yBegin[index] * (val - xBegin[index]) / (xBegin[index-1] - xBegin[index]);
				}
				else return yBegin[nSize - 1];
			}
			return  yBegin[0];
		}

		static T interp(const T* xBegin, const T* yBegin, const size_t nSize, const T& val)
		{
			int index = getIndex(xBegin, nSize, val);
			return calculateValue(xBegin, yBegin, nSize, index, val);
		}

	public:
		T operator()(const T* xBegin, const T* yBegin, const size_t nSize, const T& val) const
		{
			return interpolate::interp(xBegin, yBegin, nSize, val);
		}
	};
};


NAMESPACE_END(KOG)
#endif
