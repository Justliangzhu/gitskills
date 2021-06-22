// UltraCArray_T.h: interface for the UltraCArray_T class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ULTRACARRAY_T_H__552AA3BD_C40E_4090_B373_53767023912A__INCLUDED_)
#define AFX_ULTRACARRAY_T_H__552AA3BD_C40E_4090_B373_53767023912A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afxtempl.h>
template <class TYPE, class ARG_TYPE>
class UltraCArray_T  :  public CArray <TYPE, ARG_TYPE> 
{
public:
	UltraCArray_T();
	virtual ~UltraCArray_T();
	void operator =(const UltraCArray_T<TYPE, ARG_TYPE> &rhs);

};

template <class TYPE, class ARG_TYPE>
UltraCArray_T<TYPE, ARG_TYPE>::UltraCArray_T()
{

}

template <class TYPE, class ARG_TYPE>
UltraCArray_T<TYPE, ARG_TYPE>::~UltraCArray_T()
{

}

template <class TYPE, class ARG_TYPE>
void UltraCArray_T<TYPE, ARG_TYPE>::operator =(const UltraCArray_T<TYPE, ARG_TYPE> &rhs)
{
	RemoveAll();
	Append(rhs);
}

#endif // !defined(AFX_ULTRACARRAY_T_H__552AA3BD_C40E_4090_B373_53767023912A__INCLUDED_)






