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