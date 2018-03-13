#pragma once


// CBottomEditView view

class CBottomEditView : public CEditView
{
	DECLARE_DYNCREATE(CBottomEditView)

protected:
	CBottomEditView();           // protected constructor used by dynamic creation
	virtual ~CBottomEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


