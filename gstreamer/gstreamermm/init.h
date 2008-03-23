// -*- c++ -*-

/* gstreamermm - a C++ wrapper for gstreamer
 *
 * Copyright 2008 The gstreamermm Development Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _GSTREAMERMM_INIT_H
#define _GSTREAMERMM_INIT_H

#include <glibmm/error.h>
#include <glibmm/optiongroup.h>

namespace Gst
{

void init(int& argc, char**& argv);
bool init_check(int& argc, char**& argv, Glib::Error& error);
Glib::OptionGroup init_get_option_group();
  
}//end namespace Gst

#endif //_GSTREAMERMM_INIT_H

