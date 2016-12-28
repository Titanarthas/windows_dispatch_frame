
#ifndef __WDF_EXPORT_H_23443546575
#define __WDF_EXPORT_H_23443546575

#ifndef WDF_EXPORT
#  ifndef WDF_EXPORTS
#    define WDF_EXPORT __declspec(dllimport)
#  else
#    define WDF_EXPORT __declspec(dllexport)
#  endif  
#endif

#endif /* __WDF_EXPORT_H_23443546575 */