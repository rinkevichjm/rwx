/*
 * wxPen.hpp
 *
 *  Created on: 16.03.2012
 *      Author: hanmac
 */

#ifndef WXPEN_HPP_
#define WXPEN_HPP_

#include "main.hpp"

extern VALUE rb_cWXPen;
void Init_WXPen(VALUE rb_mWX);

template <>
VALUE wrap< wxPen >(wxPen *bitmap );

template <>
VALUE wrap< wxPen >(const wxPen &bitmap );

template <>
wxPen* unwrap< wxPen* >(const VALUE &vbitmap);

template <>
wxPen unwrap< wxPen >(const VALUE &vbitmap);


namespace RubyWX {
namespace Pen {

void define_const();

}
}

#endif /* WXPEN_HPP_ */
