set_xmakever("2.9.3")

ProjectName = "FlashlightEngine"

set_project(ProjectName)

add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")
add_rules("compiler-setup")

if is_mode("release") then
  set_fpmodels("fast")
  set_optimize("fastest")
  set_symbols("hidden")
else
  add_defines("FL_DEBUG")
  set_symbols("debug")
end

set_encodings("utf-8")
set_exceptions("cxx")
set_languages("cxx20")
set_rundir("./bin/$(plat)_$(arch)_$(mode)")
set_targetdir("./bin/$(plat)_$(arch)_$(mode)")
set_warnings("allextra")
add_cxflags("-Wno-missing-field-initializers -Werror=vla", {tools = {"clang", "gcc"}})
add_includedirs("Include")

option("override_runtime", {description = "Override VS runtime to MD in release and MDd in debug.", default = true})
option("build_static", {description = "Build the engine as a static library.", default = false})
option("unitybuild", { description = "Build the engine using unity build", default = false })

if is_plat("windows") then
  if has_config("override_runtime") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
  end
end

if has_config("build_static") then
  add_defines("FL_STATIC")
end

add_requires("fmt")

target(ProjectName, function (target)
  set_kind("shared")
  
  add_files("Source/**.cpp")
  
  for _, ext in ipairs({".hpp", ".inl"}) do
    add_headerfiles("Include/**" .. ext)
  end

  if is_plat("windows", "mingw") then
  	add_defines("FL_WINDOWS_NT6=1")
  end

  -- Remove platform-specific files
  if not is_plat("windows", "mingw") then
  	remove_headerfiles("Source/FlashlightEngine/*/Win32/**")
  	remove_files("Source/FlashlightEngine/*/Win32/**")
  end

  if not is_plat("linux", "android", "cross") then
  	remove_headerfiles("Source/FlashlightEngine/*/Linux/**")
  	remove_files("Source/FlashlightEngine/*/Linux/**")
  end

  if not is_plat("macosx", "iphoneos") then
   	remove_headerfiles("Source/FlashlightEngine/*/Darwin/**")
  	remove_files("Source/FlashlightEngine/*/Darwin/**")
  end

  if not is_plat("linux", "macosx", "iphoneos", "android", "wasm", "cross", "bsd") then
  	remove_headerfiles("Source/FlashlightEngine/*/Posix/**")
  	remove_files("Source/FlashlightEngine/*/Posix/**")
  end

  if has_config("unitybuild") then
    add_rules("c++.unity_build", {uniqueid = "FL_UNITY_ID", batchsize = 12})
  end

  add_packages("fmt")

  add_defines("FL_BUILD")
  
  add_rpathdirs("$ORIGIN")
end)

includes("xmake/**.lua") -- Include external scripts