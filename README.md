# CG-2425 - Mini Scene Graph Based 3D Engine

## Development
1. Run `./setup.sh` to setup the project.
2. Run `./build.sh` to build the entire project.
3. Run `./run.sh engine` and `./run.sh generator` to execute the built `engine` or `generator` executables.

- Use the `DEBUG_MODE` comptime define to determine whether the project is in debug mode.
- Use `./clean.sh` to remove the build artifacts. Use `./clean.sh --all` to nuke the whole build directory.

If CMake stops working, god help you, because I sure as fuck won't.  
![I'll punch you](./docs/repo/punch.png)

### Debugging
There are two VSCode debug profiles included in this project, Engine and Generator. To change the arguments passed to the executable at runtime, go to [`.vscode/launch.json`](./.vscode/launch.json) and edit the property `args` for each respective profile.

### Notes
- When creating new geometry primitives, place it's creation and serialization methods in `generator`, it's deserialization in `engine`, and it's definition in a `common` header, inheriting from `BaseGeometry`. Remember to add a default destructurer to the source files, in both solutions.

## Description

### 🎯 Purpose:

### 🚀 Key Features:

## 📚 Learning Outcomes

## 🚧 Areas for Improvement

## 👨‍💻 Contributors

## 🛠️ Technologies Used

## Final Grade

-/20