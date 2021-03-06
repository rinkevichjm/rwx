/*
 * wxBrush.hpp
 *
 *  Created on: 21.04.2012
 *      Author: hanmac
 */

#ifndef WXBRUSH_HPP_
#define WXBRUSH_HPP_


#include "main.hpp"

extern VALUE rb_cWXBrush;
void Init_WXBrush(VALUE rb_mWX);

template <>
VALUE wrap< wxBrush >(wxBrush *bitmap );

template <>
VALUE wrap< wxBrush >(const wxBrush &bitmap );

template <>
wxBrush unwrap< wxBrush >(const VALUE &vbrush);

namespace RubyWX {
namespace Brush {

void define_const();

}
}

#endif /* WXBRUSH_HPP_ */
