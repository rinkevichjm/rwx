/*
 * wxAuiToolBar.cpp
 *
 *  Created on: 30.03.2012
 *      Author: hanmac
 */


#include "wxAuiToolBar.hpp"
#include "wxToolBarBase.hpp"

VALUE rb_cWXAuiToolBar;

#undef wxUSE_AUI
#if wxUSE_AUI
#define _self unwrap<wxAuiToolBar*>(self)

namespace RubyWX {
namespace AuiToolBar {

APP_PROTECT(wxAuiToolBar)

DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "11",&parent,&hash);
	_self->Create(unwrap<wxWindow*>(parent),wxID_ANY);
	
	rb_call_super(argc,argv);
	return self;
}

}
}

#endif

DLL_LOCAL void Init_WXAuiToolBar(VALUE rb_mWX)
{
#if wxUSE_AUI
	using namespace RubyWX::AuiToolBar;
	rb_cWXAuiToolBar = rb_define_class_under(rb_mWX,"AuiToolBar",rb_cWXToolBarBase);
	rb_define_alloc_func(rb_cWXAuiToolBar,_alloc);

	rb_define_method(rb_cWXAuiToolBar,"initialize",RUBY_METHOD_FUNC(_initialize),-1);
#endif
}
