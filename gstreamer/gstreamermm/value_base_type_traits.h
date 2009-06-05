// -*- c++ -*-

/* gstreamermm - a C++ wrapper for gstreamer
 *
 * Copyright 2008-2009 The gstreamermm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <glibmm/containerhandle_shared.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace Glib
{

namespace Container_Helpers
{

template <>
struct TypeTraits<Glib::ValueBase>
{
  typedef Glib::ValueBase       CppType;
  typedef GValue *              CType;
  typedef GValue *              CTypeNonConst;

  static CType to_c_type (Glib::ValueBase& v)
    { return v.gobj(); }

  static CType to_c_type (CType v)
    { return v; }

  static CppType to_cpp_type(CType v)
    { ValueBase b; if (v) b.init(v); return b; }

  // Glib::~ValueBase() frees underlying C type already.
  static void release_c_type(CType v) { }
};

}

}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
