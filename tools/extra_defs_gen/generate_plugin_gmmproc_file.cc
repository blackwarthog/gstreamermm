/* $Id: generate_extra_defs.cc 740 2008-10-15 15:58:17Z jaalburqu $ */

/* generate_extra_defs.cc
 *
 * Copyright (C) 2001 The Free Software Foundation
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


#include <gst/gst.h>
#include <glibmm.h>
#include <iostream>

static gchar* nmspace;
static gchar* defsFile;
static gchar* target;

static Glib::ustring pluginName;
static Glib::ustring cTypeName;
static Glib::ustring cParentTypeName;
static Glib::ustring cppTypeName;
static Glib::ustring cppParentTypeName;
static Glib::ustring castMacro;

Glib::ustring get_cast_macro(const Glib::ustring& typeName)
{
  Glib::ustring result;

  Glib::ustring::const_iterator iter = typeName.begin();

  if (iter != typeName.end())
    result.push_back(*iter);

  for ( ++iter; iter != typeName.end(); ++iter)
  {
    if (g_unichar_isupper(*iter))
    {
      result.push_back('_');
    }
    result.push_back(g_unichar_toupper(*iter));
  }

  return result;
}

void generate_hg_file()
{
  std::cout << "#include <" << target << "/" <<
    cppParentTypeName.lowercase() << ".h>" << std::endl << std::endl;

  std::cout << "_DEFS(" << target << "," << defsFile << ")" << std::endl <<
    std::endl;

  std::cout << "namespace " << nmspace << std::endl;
  std::cout << "{" << std::endl << std::endl;

  std::cout << "/** " << nmspace << "::" << cppTypeName << " — " << pluginName << " plugin." << std::endl;
  std::cout << " * Please include <" << target << "/" << cppTypeName.lowercase() << ".h> to use." << std::endl;
  std::cout << " */" << std::endl;
  std::cout << "class " << cppTypeName << std::endl;
  std::cout << ": public " << cppParentTypeName << std::endl;
  std::cout << "{" << std::endl;
  std::cout << "  _CLASS_GOBJECT(" << cppTypeName << ", " << cTypeName <<
    ", " << castMacro << ", " << cppParentTypeName <<
    ", " << cParentTypeName << ")" << std::endl << std::endl;

  std::cout << "protected:" << std::endl;
  std::cout << "  " << cppTypeName << "();" << std::endl;
  std::cout << "  " << cppTypeName << "(const Glib::ustring& name);" << std::endl << std::endl;

  std::cout << "public:" << std::endl;
  std::cout << "/** Creates a new " << pluginName << " plugin with a unique name." << std::endl;
  std::cout << " */" << std::endl;
  std::cout << "  _WRAP_CREATE()" << std::endl << std::endl;

  std::cout << "/** Creates a new " << pluginName << " plugin with the given name." << std::endl;
  std::cout << " */" << std::endl;
  std::cout << "  _WRAP_CREATE(const Glib::ustring& name)" << std::endl;
  std::cout << "};" << std::endl;

  std::cout << std::endl << "} //namespace " << nmspace << std::endl;
}

void generate_ccg_file()
{
  std::cout << "_PINCLUDE(" << target << "/private/" <<
    cppParentTypeName.lowercase() << "_p.h)" << std::endl;
  std::cout << "#include <glib/gprintf.h>" << std::endl << std::endl;

  Glib::ustring getTypeName = castMacro.lowercase() + "_get_type";

  std::cout << "extern \"C\" GType " << getTypeName << "();" << std::endl;
  std::cout << "GType " << getTypeName << "()" << std::endl;
  std::cout << "{" << std::endl;
  std::cout << "  static GType type = 0;" << std::endl;
  std::cout << "  GstElementFactory* factory = 0;" << std::endl;
  std::cout << "  GstPluginFeature* feature = 0;" << std::endl << std::endl;

  std::cout << "  if (!type)" << std::endl; std::cout << "  {" << std::endl;
  std::cout << "    factory = gst_element_factory_find(\"" << pluginName << "\");" << std::endl;

  std::cout << "    // Make sure that the feature is actually loaded:" << std::endl;
  std::cout << "    if (factory)" << std::endl;
  std::cout << "    {" << std::endl;
  std::cout << "      feature =" << std::endl;
  std::cout << "        gst_plugin_feature_load(GST_PLUGIN_FEATURE(factory));" << std::endl << std::endl;

  std::cout << "      g_object_unref(factory);" << std::endl;
  std::cout << "      factory = GST_ELEMENT_FACTORY(feature);" << std::endl;
  std::cout << "      type = gst_element_factory_get_element_type(factory);" << std::endl;
  std::cout << "      g_object_unref(factory);" << std::endl;
  std::cout << "    }" << std::endl;
  std::cout << "  }" << std::endl << std::endl;

  std::cout << "  return type;" << std::endl;
  std::cout << "}" << std::endl << std::endl;

  std::cout << "namespace " << nmspace << std::endl;
  std::cout << "{" << std::endl << std::endl;

  std::cout << cppTypeName << "::" << cppTypeName << "()" << std::endl;
  std::cout << ": _CONSTRUCT(\"name\", NULL)" << std::endl;
  std::cout << "{}" << std::endl << std::endl;

  std::cout << cppTypeName << "::" << cppTypeName << "(const Glib::ustring& name)" << std::endl;
  std::cout << ": _CONSTRUCT(\"name\", name.c_str())" << std::endl;
  std::cout << "{}" << std::endl;

  std::cout << std::endl << "}" << std::endl;
}

int main(int argc, char* argv[])
{
  gboolean hgFile = false;
  gboolean ccgFile = false;

  if (!g_thread_supported())
    g_thread_init(NULL);

  GOptionEntry optionEntries[] =
  {
    {"hg", 'h', 0, G_OPTION_ARG_NONE, &hgFile, "Generate .hg file", NULL },
    {"ccg", 'c', 0, G_OPTION_ARG_NONE, &ccgFile, "Generate .ccg file", NULL },
    {"namespace", 'n', 0, G_OPTION_ARG_STRING, &nmspace, "The namespace of the plugin", "namespace" },
    {"main-defs", 'm', 0, G_OPTION_ARG_STRING, &defsFile, "The main .defs file without .defs extension (used in _DEFS() directive)", "def" },
    {"target", 't', 0, G_OPTION_ARG_STRING, &target, "The target directory  of the generated .h and .cc files (used in _DEFS() directive)", "directory" },
    { NULL }
  };

  GOptionContext* gContext = g_option_context_new("<plugin name>");
  g_option_context_set_summary(gContext, "Outputs a GStreamer plugin's gmmproc files to be processed by gmmproc for wrapping in gstreamermm.  Use the same syntax for the plugin name as in gst-inspect.");

  g_option_context_add_main_entries(gContext, optionEntries, NULL);
  g_option_context_add_group(gContext, gst_init_get_option_group());

  Glib::OptionContext optionContext(gContext, true);

  try
  {
    if (!optionContext.parse(argc, argv))
    {
      std::cout << "Error parsing options and initializing.  Sorry." <<
        std::endl;
      return -1;
    }
  }
  catch (Glib::OptionError& error)
  {
      std::cout << "Error parsing options and initializing GStreamer." <<
        std::endl << "Run `" << argv[0] << " -?'  for a list of options." <<
        std::endl;
      return -1;
  }

  if (argc != 2)
  {
    std::cout << "A plugin name must be supplied to generate an .hg file." <<
      std::endl << "Run `" << argv[0] << " -?'  for a list of options." <<
      std::endl;
    return -1;
  }

  GType type = 0;
  GstElementFactory* factory = 0;

  factory = gst_element_factory_find(argv[1]);

  // Make sure that the feature is actually loaded:
  if (factory)
  {
    GstPluginFeature* loaded_feature =
            gst_plugin_feature_load(GST_PLUGIN_FEATURE(factory));

    g_object_unref(factory);
    factory = GST_ELEMENT_FACTORY(loaded_feature);
    type = gst_element_factory_get_element_type(factory);

    pluginName = argv[1];
    cTypeName = g_type_name(type);
    cParentTypeName = g_type_name(g_type_parent(type));
    cppTypeName = cTypeName.substr(3);
    cppParentTypeName = cParentTypeName.substr(3);
    castMacro = get_cast_macro(cTypeName);

    if (hgFile)
      generate_hg_file();
    else if (ccgFile)
      generate_ccg_file();

    g_object_unref(factory);
  }

  return 0;
}