/**
 * @brief   调试工具
 * @author  dongbo
 * @date    2010-3-17
 * @remarks
**/

#include "debug_tool.hpp"

#include <windows.h>
#include <WinDNS.h>
#include <DbgHelp.h>
#include <Psapi.h>
#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "Psapi.lib" )

namespace dbsoft
{
	namespace detail
	{
		class callstack_Imp
		{
		public:
			typedef callstack::func_name					       func_name;
			typedef callstack::func_name_list                      func_name_list;
			typedef callstack::const_iterator                      const_iterator;
			typedef callstack::callstack_ptr                       callstack_ptr;

		public:
			callstack_Imp();
			~callstack_Imp() {}

		public:
			const_iterator begin() const
			{
				return m_lstFunc.begin();
			}

			const_iterator end() const
			{
				return m_lstFunc.end();
			}

		private:
			func_name_list   m_lstFunc;

		public:
			static callstack_ptr  generate( const void* pContext );
		protected:
			static void           _initialize();
			static bool           _loadAllModules();
			static void           _stackwalk( QWORD* pTrace, DWORD dwMaxDepth, CONTEXT* pContext );
			static func_name      _getfuncname( QWORD dwFunc );
		private:
			static bool           m_bInitialized;
		};

		bool callstack_Imp::m_bInitialized = false;

		callstack_Imp::callstack_Imp()
		{
			if( !m_bInitialized )
			{
				_initialize();
			}
		}

		void callstack_Imp::_stackwalk(QWORD *pTrace, DWORD dwMaxDepth, CONTEXT *pContext)
		{
			STACKFRAME64        sfStackFrame64;
			HANDLE              hProcess = ::GetCurrentProcess();
			HANDLE              hThread  = ::GetCurrentThread();

			DWORD               dwDepth  = 0;

			::ZeroMemory( &sfStackFrame64, sizeof(sfStackFrame64) );

			__try
			{
				sfStackFrame64.AddrPC.Offset        = pContext->Eip;
				sfStackFrame64.AddrPC.Mode          = AddrModeFlat;
				sfStackFrame64.AddrStack.Offset     = pContext->Esp;
				sfStackFrame64.AddrStack.Mode       = AddrModeFlat;
				sfStackFrame64.AddrFrame.Offset     = pContext->Ebp;
				sfStackFrame64.AddrFrame.Mode       = AddrModeFlat;

				bool bSuccessed = true;

				while( bSuccessed && (dwDepth < dwMaxDepth) )
				{
					bSuccessed = ::StackWalk64( 
						IMAGE_FILE_MACHINE_I386,
						hProcess,
						hThread,
						&sfStackFrame64,
						pContext,
						NULL,
						SymFunctionTableAccess64,
						SymGetModuleBase64,
						NULL
						) != FALSE ;

					pTrace[ dwDepth ] = sfStackFrame64.AddrPC.Offset;
					++dwDepth;

					if( !bSuccessed )
					{
						break;
					}

					if( sfStackFrame64.AddrFrame.Offset == 0 )
					{
						break;
					}
				}
			}
			__except( EXCEPTION_EXECUTE_HANDLER )
			{
			}
		}

		callstack_Imp::func_name callstack_Imp::_getfuncname( QWORD dwFunc )
		{
			static const int gc_iMaxNameLength = 4096;
			char                    szSymbol[ sizeof(IMAGEHLP_SYMBOL64) + gc_iMaxNameLength ] = {0};
			PIMAGEHLP_SYMBOL64      Symbol;
			DWORD                   dwSymbolDisplacement = 0;
			DWORD64                 dw64SymbolDisplacement = 0;

			HANDLE                  hProcess = ::GetCurrentProcess();

			Symbol                  = (PIMAGEHLP_SYMBOL64)szSymbol;
			Symbol->SizeOfStruct    = sizeof(szSymbol);
			Symbol->MaxNameLength   = gc_iMaxNameLength;

			func_name              strResult;

			if( ::SymGetSymFromAddr64( 
				hProcess,
				dwFunc,
				&dw64SymbolDisplacement,
				Symbol )
				)
			{
				int i=0;
				for( ; Symbol->Name[i] < 32 || Symbol->Name[i] > 127; )
				{
					++i;
				}

				strResult += ( const char* )( Symbol->Name + i );
			}
			else
			{
				return "unknown function";
			}

	 		IMAGEHLP_LINE64            ImageHelpLine;
	 		ImageHelpLine.SizeOfStruct = sizeof(ImageHelpLine);
	 
	 		if( ::SymGetLineFromAddr64( hProcess, dwFunc, &dwSymbolDisplacement, &ImageHelpLine ) )
	 		{
				// 还是删掉吧，虽然这个操作很好用，但是过多的依赖我不想看到
	 			// fmt::WritePipe( strResult, "文件: ", ImageHelpLine.FileName, " 行数：", ImageHelpLine.LineNumber );

				// 我就不信，4096都不够
				char szBuf[4096] = {0};
				sprintf_s( szBuf, "file:%s, line:%d\n", ImageHelpLine.FileName, ImageHelpLine.LineNumber );

				strResult += szBuf;
	 		}

			IMAGEHLP_MODULE64  ImageHelpModule;
			ImageHelpModule.SizeOfStruct = sizeof(ImageHelpModule);

			if( ::SymGetModuleInfo64( hProcess, dwFunc, &ImageHelpModule ) )
			{
				char               szModuleName[1024];
				sprintf_s( szModuleName, " model: %s", ImageHelpModule.ImageName );

				strResult += szModuleName;
			}

			return strResult;
		}

		callstack_Imp::callstack_ptr callstack_Imp::generate( const void* pContext )
		{
			if( !m_bInitialized )
			{
				_initialize();
			}

			CONTEXT Context;

			if( pContext != NULL )
			{
				::memcpy_s( &Context, sizeof(CONTEXT), pContext, sizeof(CONTEXT) );
			}
			else
			{
				::ZeroMemory( &Context, sizeof(Context) );
				Context.ContextFlags = CONTEXT_FULL;

				__asm
				{
					call FakeFuncCall
FakeFuncCall:

					pop eax
						mov Context.Eip, eax
						mov Context.Ebp, ebp
						mov Context.Esp, esp
				}
			}

			static const int gc_iMaxStackDepth = 512;
			QWORD aryStack[gc_iMaxStackDepth] = {0};

			// 由于_stackwalk内部使用SEH 因此不能在其内部使用C++类
			_stackwalk( aryStack, gc_iMaxStackDepth, &Context );

			callstack_ptr spCallStack( new callstack() );

			for( int i=0; i<gc_iMaxStackDepth && aryStack[i] != 0; ++i )
			{
				func_name name = _getfuncname(aryStack[i]);
				spCallStack->m_spImp->m_lstFunc.push_back( name );
			}		

			return spCallStack;
		}

		void callstack_Imp::_initialize()
		{
			if( m_bInitialized )
			{
				return;
			}

			// 设置符号引擎
			DWORD SymOpts = ::SymGetOptions();
			SymOpts |= SYMOPT_LOAD_LINES;
			SymOpts |= SYMOPT_DEBUG;
			::SymSetOptions( SymOpts );

			if( FALSE == ::SymInitialize( ::GetCurrentProcess(), NULL, TRUE ) )
			{
				//	DBSOFT_LogMsg( "::SymInitialize初始化失败..." );
				return;
			}

			if( !_loadAllModules() )
			{
				//	DBSOFT_LogMsg( "LoadModules发生了错了" );
			}

			m_bInitialized = true;
		}

		bool callstack_Imp::_loadAllModules()
		{
			HANDLE                 hProcess = ::GetCurrentProcess();
			static const int       gc_iMaxHandles = 4096;
			HMODULE                aryHandles[ gc_iMaxHandles ] = {0};

			unsigned               uBytes = 0;

			BOOL bResult = ::EnumProcessModules(
				hProcess, aryHandles, sizeof(aryHandles), (LPDWORD)&uBytes );

			if( FALSE == bResult )
			{
				//	DBSOFT_LogMsg( "::EnumProcessModules失败 Msg:", GetLastErrorDescA() );
				return false;
			}

			const int iCount = uBytes/sizeof(HMODULE);

			for( int i=0; i < iCount; ++i )
			{
				char szModuleName[4096] = {0};
				char szImageName[4096] ={0};
				MODULEINFO  Info;

				::GetModuleInformation( hProcess, aryHandles[i], &Info, sizeof(Info) );
				::GetModuleFileNameExA( hProcess, aryHandles[i], szImageName, 4096 );
				::GetModuleBaseNameA( hProcess, aryHandles[i], szModuleName, 4096 );

				::SymLoadModule64( hProcess, aryHandles[i], szImageName, szModuleName, (DWORD64)Info.lpBaseOfDll, (DWORD)Info.SizeOfImage );
			}

			return true;
		}
	}
	
	callstack::callstack():m_spImp( new detail::callstack_Imp() )
	{		
	}

	callstack::~callstack()
	{
		if (m_spImp != NULL)
		{
			delete m_spImp;
			m_spImp = NULL;
		}
	}

	callstack::const_iterator callstack::begin() const
	{
		return m_spImp->begin();
	}

	callstack::const_iterator callstack::end() const
	{
		return m_spImp->end();
	}

	callstack::callstack_ptr callstack::generate( const void* pContext )
	{
		return detail::callstack_Imp::generate( pContext );
	}
}