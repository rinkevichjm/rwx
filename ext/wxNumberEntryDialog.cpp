/*
 * wxNumberEntryDialog.cpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */


#include "wxNumberEntryDialog.hpp"
#include "wxPoint.hpp"
#include "wxApp.hpp"

VALUE rb_cWXNumberEntryDialog;
#if wxUSE_NUMBERDLG
#define _self unwrap<wxNumberEntryDialog*>(self)

namespace RubyWX {
namespace NumberEntryDialog {

#ifdef HAVE_WXNUMBERENTRYDIALOG
APP_PROTECT(wxNumberEntryDialog)
#else
APP_PROTECT_NULL
#endif

singlereturn(GetValue)

/*
 * call-seq:
 *   NumberEntryDialog.new(parent, name, [options])
 *   NumberEntryDialog.new(parent, [options])
 *
 * creates a new NumberEntryDialog widget.
 * ===Arguments
 * * parent of this window or nil
 *
 * *options: Hash with possible options to set:
 *   * path String default path
 *   * message String
 *
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(hash,rb_cString))
	{
		wxString message,prompt,caption;
		long value = 0, min = 0, max = 0;
		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"prompt",prompt);
			set_hash_option(hash,"caption",caption);

			set_hash_option(hash,"value",value);
			set_hash_option(hash,"min",min);
			set_hash_option(hash,"max",max);
		}
#ifdef HAVE_WXNUMBERENTRYDIALOG
		_self->Create(unwrap<wxWindow*>(parent),
				message,prompt,caption,value,min,max);
#else
		RTYPEDDATA_DATA(self) = new wxNumberEntryDialog(unwrap<wxWindow*>(parent),
				message,prompt,caption,value,min,max);
#endif
		
	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _getNumber(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "02",&parent,&hash);

	app_protected();

	wxString message,prompt,caption;
	long value(0), min(0), max(100);
	wxPoint pos(wxDefaultPosition);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"prompt",prompt);
		set_hash_option(hash,"caption",caption);

		set_hash_option(hash,"value",value);
		set_hash_option(hash,"min",min);
		set_hash_option(hash,"max",max);

		set_hash_option(hash,"pos",pos);
	}

	return wrap(wxGetNumberFromUser(
			message,prompt,caption,
			value,min,max,
			unwrap<wxWindow*>(parent),pos));
}

}
}

#endif

DLL_LOCAL void Init_WXNumberEntryDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);
#endif

#if wxUSE_NUMBERDLG
	using namespace RubyWX::NumberEntryDialog;
	rb_cWXNumberEntryDialog = rb_define_class_under(rb_mWX,"NumberEntryDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXNumberEntryDialog,_alloc);
	rb_define_method(rb_cWXNumberEntryDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_method(rb_cWXNumberEntryDialog,"value",RUBY_METHOD_FUNC(_GetValue),0);

	rb_define_module_function(rb_mWX,"number_dialog",RUBY_METHOD_FUNC(_getNumber),-1);

	registerInfo<wxNumberEntryDialog>(rb_cWXNumberEntryDialog);
#endif
}
