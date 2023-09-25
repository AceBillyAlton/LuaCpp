toolchain("CustomClang")
    set_toolset("cc", "clang")
    set_toolset("cxx", "clang", "clang++")
toolchain_end()

add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_requires("lua 5.4")

set_languages("c17", "c++23")
set_rundir("$(projectdir)/")
target("LuaCpp")
    set_kind("binary")
    add_files("src/cpp/*.cpp")
    add_packages("lua")
    set_toolchains("CustomClang")