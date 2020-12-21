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