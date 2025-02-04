if has_config("build_tests") then
  add_requires("catch2")
  
  target("FlTests")
    set_kind("binary")
    
    add_files("Source/**.cpp")

    add_rpathdirs("$ORIGIN")

    add_packages("catch2")

    add_deps("FlashlightEngine")
end