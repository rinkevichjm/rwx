/*
 * wxtextCtrl.hpp
 *
 *  Created on: 28.04.2012
 *      Author: hanmac
 */

#ifndef WXTEXTCTRL_HPP_
#define WXTEXTCTRL_HPP_

#include "wxControl.hpp"

extern VALUE rb_cWXTextCtrl;

void Init_WXTextCtrl(VALUE rb_mWX);

#if wxUSE_TEXTCTRL
#include <wx/textctrl.h>
#endif

namespace RubyWX {
namespace TextCtrl {

void set_style_flags(VALUE hash,int& flags);

}
}


#endif /* WXTEXTCTRL_HPP_ */
