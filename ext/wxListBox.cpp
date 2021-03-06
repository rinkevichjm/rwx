/*
 * wxListBox.cpp
 *
 *  Created on: 14.03.2013
 *      Author: hanmac
 */

#include "wxListBox.hpp"
#include "wxItemContainer.hpp"

VALUE rb_cWXListBox;

#if wxUSE_LISTBOX

namespace RubyWX {
namespace ListBox {
#define _self unwrap<wxListBox*>(self)

APP_PROTECT(wxListBox)


void set_style_flags(VALUE hash,int& flags)
{
	set_hash_flag_option(hash,"sort",wxLB_SORT,flags);
	set_hash_flag_option(hash,"multiple",wxLB_MULTIPLE,flags);
	set_hash_flag_option(hash,"extended",wxLB_EXTENDED,flags);

}


/*
 * call-seq:
 *   ListBox.new(parent, name, [options])
 *   ListBox.new(parent, [options])
 *
 * creates a new ListBox widget.
 * ===Arguments
 * * parent of this window or nil
 * * name is a String describing a resource in a loaded xrc
 *
 * * options: Hash with possible options to set:
 *   * sort true/false adds the SortStyle to make this
*/
DLL_LOCAL VALUE _initialize(int argc,VALUE *argv,VALUE self)
{
	VALUE parent,name,hash;
	rb_scan_args(argc, argv, "11:",&parent,&name,&hash);
	if(!_created && !rb_obj_is_kind_of(name,rb_cString)) {
		wxWindowID id(wxID_ANY);
		wxArrayString choices;
		int style(0);

		if(rb_obj_is_kind_of(hash,rb_cHash)) {
			set_hash_option(hash,"id",id,unwrapID);
			set_hash_option(hash,"choices",choices);
			set_hash_option(hash,"style",style);

			set_style_flags(hash,style);
		}
		if(nil_check(parent)) {
			_self->Create(
				unwrap<wxWindow*>(parent),id,
				wxDefaultPosition,wxDefaultSize,
				choices,style
			);
		}
	}

	rb_call_super(argc,argv);

	if(rb_obj_is_kind_of(hash,rb_cHash)) {
		set_ruby_option(hash, "selection", ItemContainer::_setSelection, self);
	}

	return self;
}

DLL_LOCAL VALUE _getSelections(VALUE self)
{
	if(_self->HasMultipleSelection())
	{
		wxArrayInt data;
		_self->GetSelections(data);
		return wrap(data);
	} else {
		int val = _self->GetSelection();
		return val == wxNOT_FOUND ? Qnil : wrap(val);
	}

}


DLL_LOCAL VALUE _setSelections(VALUE self,VALUE val)
{
	rb_check_frozen(self);

	if(NIL_P(val)) {
		_self->SetSelection(wxNOT_FOUND);
	} else {
		wxArrayInt data(unwrap<wxArrayInt>(val));

		for(wxArrayInt::iterator it = data.begin();it != data.end();++it)
			if(check_index(*it,_self->GetCount()))
				_self->SetSelection(*it);
	}
	return val;
}


DLL_LOCAL VALUE _getStringSelections(VALUE self)
{
	if(_self->HasMultipleSelection())
	{
		wxArrayInt data;
		wxArrayString result;
		_self->GetSelections(data);
		for(wxArrayInt::iterator it = data.begin(); it != data.end();++it)
			result.push_back(_self->GetString(*it));
		return wrap(result);
	} else {
		return wrap(_self->GetStringSelection());
	}

}


DLL_LOCAL VALUE _setStringSelection(VALUE self,VALUE val)
{
	rb_check_frozen(self);

	if(NIL_P(val)) {
		_self->SetSelection(wxNOT_FOUND);
	} else {
		wxArrayString data(unwrap<wxArrayString>(val));

		for(wxArrayString::iterator it = data.begin();it != data.end();++it)
			_self->SetStringSelection(*it);
	}
	return val;
}

DLL_LOCAL VALUE _each_selection_size(VALUE self)
{
	wxArrayInt data;
	return INT2NUM(_self->GetSelections(data));
}

/*
 * call-seq:
 *   each_selection -> Enumerator
 *   each_selection { |idx, text| } -> self
 *
 * iterates the selected items of the list box.
 * ===Return value
 * self
 *
*/
DLL_LOCAL VALUE _each_selection(VALUE self)
{
	RETURN_SIZED_ENUMERATOR(self,0,NULL,RUBY_METHOD_FUNC(_each_selection_size));
	wxArrayInt data;
	_self->GetSelections(data);
	for(wxArrayInt::iterator it = data.begin(); it != data.end();++it)
		rb_yield_values(2,INT2NUM(*it),wrap(_self->GetString(*it)));
	return self;
}

macro_attr_item(ItemSelection,IsSelected, SetSelection, GetCount, bool)

}
}

#endif


/* Document-method: get_item_selection
 * call-seq:
 *   get_item_selection(pos) -> true/false
 *
 * Determines whether an item is selected.
 * ===Arguments
 * * pos of the item. Integer
 * ===Return value
 * true/false
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-method: set_item_selection
 * call-seq:
 *   set_item_selection(pos,val) -> self
 *
 * sets the item at the position as selected if val is true.
 * ===Arguments
 * * pos of the item. Integer
 * * val if item should be selected. true/false
 * ===Return value
 * self
 * === Exceptions
 * [IndexError]
 * * pos is greater than the count of items
 *
*/

/* Document-attr: selection
 * Integer/Array/nil returns the index of the current selected item,
 * or nil if none is selected.
 * Array is returned if multiple flag is used.
 */
/* Document-attr: string_selection
 * String/Array returns the string of the current selected item.
 * Array is returned if multiple flag is used.
 */

/* Document-const: SORT
 *   The listbox contents are sorted in alphabetical order.
 */
/* Document-const: SINGLE
 *   Single-selection list.
 */
/* Document-const: MULTIPLE
 *   Creates a text control to the left of the picker which is completely managed by this WX::PickerBase class.
 */
/* Document-const: EXTENDED
 *   Extended-selection list: the user can extend the selection by using SHIFT or CTRL keys together with the cursor movement keys or the mouse.
 */


DLL_LOCAL void Init_WXListBox(VALUE rb_mWX)
{
#if 0
	rb_mWX = rb_define_module("WX");
	rb_cWXWindow = rb_define_class_under(rb_mWX,"Window",rb_cObject);

	rb_cWXControl = rb_define_class_under(rb_mWX,"Control",rb_cWXWindow);

	rb_mWXItemContainer = rb_define_module_under(rb_mWX,"ItemContainer");

#endif
#if wxUSE_LISTBOX
	using namespace RubyWX::ListBox;
	rb_cWXListBox = rb_define_class_under(rb_mWX,"ListBox",rb_cWXControl);
	rb_define_alloc_func(rb_cWXListBox,_alloc);

#if 0
	rb_define_attr(rb_cWXListBox,"selection",1,1);
	rb_define_attr(rb_cWXListBox,"string_selection",1,1);
#endif

	rb_define_method(rb_cWXListBox,"initialize",RUBY_METHOD_FUNC(_initialize),-1);

	rb_include_module(rb_cWXListBox,rb_mWXItemContainer);

	rb_define_method(rb_cWXListBox,"each_selection",RUBY_METHOD_FUNC(_each_selection),0);

	rb_define_attr_method(rb_cWXListBox,"selection",_getSelections,_setSelections);

	rb_define_attr_method(rb_cWXListBox,"string_selection",_getStringSelections,_setStringSelection);

	rb_define_method(rb_cWXListBox,"get_item_selection",RUBY_METHOD_FUNC(_getItemSelection),1);
	rb_define_method(rb_cWXListBox,"set_item_selection",RUBY_METHOD_FUNC(_setItemSelection),2);

	registerEventType("listbox", wxEVT_LISTBOX,rb_cWXCommandEvent);
	registerEventType("listbox_dclick",  wxEVT_LISTBOX_DCLICK,rb_cWXCommandEvent);

	rb_define_const(rb_cWXListBox,"SORT",INT2NUM(wxLB_SORT));
	rb_define_const(rb_cWXListBox,"SINGLE",INT2NUM(wxLB_SINGLE));
	rb_define_const(rb_cWXListBox,"MULTIPLE",INT2NUM(wxLB_MULTIPLE));
	rb_define_const(rb_cWXListBox,"EXTENDED",INT2NUM(wxLB_EXTENDED));

	registerInfo<wxListBox>(rb_cWXListBox);
#endif

}


