/*
 * wxTextEntryDialog.cpp
 *
 *  Created on: 24.12.2013
 *      Author: hanmac
 */


#include "wxTextEntryDialog.hpp"
#include "wxPoint.hpp"
#include "wxApp.hpp"

VALUE rb_cWXTextEntryDialog;
#if wxUSE_TEXTDLG
#define _self unwrap<wxTextEntryDialog*>(self)

namespace RubyWX {
namespace TextEntryDialog {

APP_PROTECT(wxTextEntryDialog)

macro_attr(Value,wxString)

/*
 * call-seq:
 *   TextEntryDialog.new(parent, name, [options])
 *   TextEntryDialog.new(parent, [options])
 *
 * creates a new TextEntryDialog widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
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
	if(!_created && !rb_obj_is_kind_of(name,rb_cString))
	{
		wxString message(wxEmptyString);
		wxString caption(wxGetTextFromUserPromptStr);
		wxString value(wxEmptyString);
		int style(wxTextEntryDialogStyle);

		if(rb_obj_is_kind_of(hash,rb_cHash))
		{
			set_hash_option(hash,"message",message);
			set_hash_option(hash,"caption",caption);

			set_hash_option(hash,"value",value);

			set_hash_option(hash,"style",style);

			TopLevel::set_style_flags(hash,style);
		}
		_self->Create(unwrap<wxWindow*>(parent),
				message,caption,value,style);
		
	}
	rb_call_super(argc,argv);

	return self;
}

DLL_LOCAL VALUE _getText(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,hash;
	rb_scan_args(argc, argv, "01:",&parent,&hash);

	app_protected();

	wxString message;
	wxString caption(wxGetTextFromUserPromptStr);
	wxString value;
	wxPoint pos(wxDefaultPosition);
	bool centre(true);

	if(rb_obj_is_kind_of(hash,rb_cHash))
	{
		set_hash_option(hash,"message",message);
		set_hash_option(hash,"caption",caption);

		set_hash_option(hash,"value",value);

		set_hash_option(hash,"pos",pos);
		set_hash_option(hash,"center",centre);
	}

	return wrap(wxGetTextFromUser(
		message,caption,
		value, unwrap<wxWindow*>(parent),
		pos.x,pos.y,centre));
}

}
}

#endif

/* Document-const: DEFAULT_STYLE
 * default style for this control.
 */

/* Document-attr: value
 * the value of the TextEntryDialog. String
 */
DLL_LOCAL void Init_WXTextEntryDialog(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXTopLevel = rb_define_class_under(rb_mWX,"TopLevel",rb_cWXWindow);
	rb_cWXDialog = rb_define_class_under(rb_mWX,"Dialog",rb_cWXTopLevel);

	rb_define_attr(rb_cWXTextEntryDialog,"value",1,1);
#endif
#if wxUSE_TEXTDLG
	using namespace RubyWX::TextEntryDialog;
	rb_cWXTextEntryDialog = rb_define_class_under(rb_mWX,"TextEntryDialog",rb_cWXDialog);
	rb_define_alloc_func(rb_cWXTextEntryDialog,_alloc);
	rb_define_method(rb_cWXTextEntryDialog,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_define_attr_method(rb_cWXTextEntryDialog,"value",_getValue,_setValue);

	rb_define_module_function(rb_mWX,"text_dialog",RUBY_METHOD_FUNC(_getText),-1);

	rb_define_const(rb_cWXTextEntryDialog,"DEFAULT_STYLE",INT2NUM(wxTextEntryDialogStyle));

	registerInfo<wxTextEntryDialog>(rb_cWXTextEntryDialog);
#endif
}
