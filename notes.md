# Notes

+ To remove the annoying red squiggles in vscode, add those paths to include path list:
    `settings.json`
    ```json
    {
        "C_Cpp.default.includePath": [
            "/home/ubuntu/.local/include",
            "${workspaceFolder}/**"
        ]
    }
    ```

+ A Lambda can capture only two kind of things: `this` pointer and non-static local variables. *[reference](https://stackoverflow.com/questions/12944002/capture-by-value-class-members)*

+ Output of `add_custom_command` can be used by `add_executable` in the same `CMakeLists.txt`. However, if `add_executable` in the sub-directory wants to use files outputted by `add_custom_command`, it will complain something like 'cannot find source file' during configure-time.

  My workaround is to use `add_library` to build those outputs as a library and not include them in `add_executable`, instead, link the library after `add_executable`. And the most important is to use `set_source_files_properties(${files} PROPERTIES GENERATED TRUE)`.

+ when installing grpc from source code, use `cmake -DgRPC_BUILD_TESTS=ON` to build tests also. and in this process, I encountered an error that `C++11 or greater detected. Should be C++03.` (`third_party/benchmark/test/cxx03_test.cc`). my solution is to comment out that line.

+ if compiler cannot know yet whether a name is a template or not when parsing the code, we need to add `template` keywork manually (just like `typename` keyword). *[reference](https://stackoverflow.com/questions/610245/where-and-why-do-i-have-to-put-the-template-and-typename-keywords)*

+ add an indirect level under `include` dir with name of project to avoid potential name conflict:
    ```c++
    #include <sailgame/common/game_manager.h>
    ```