/*
 * wxBookCtrl.hpp
 *
 *  Created on: 27.03.2012
 *      Author: hanmac
 */

#ifndef WXBOOKCTRL_HPP_
#define WXBOOKCTRL_HPP_

#include "wxEvtHandler.hpp"

extern VALUE rb_cWXBookCtrlBase,rb_cWXBookCtrlEvent;

void Init_WXBookCtrl(VALUE rb_mWX);

#if wxUSE_BOOKCTRL
template <>
wxBookCtrlBase* unwrap<wxBookCtrlBase*>(const VALUE &arg);
#endif

#endif /* WXBOOKCTRL_HPP_ */
