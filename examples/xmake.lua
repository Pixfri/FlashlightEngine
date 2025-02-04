if has_config("build_examples") then
  target("BasicApplication")
    set_kind("binary")

    add_files("BasicApplication/Source/**.cpp")

    add_deps("FlashlightEngine")
end