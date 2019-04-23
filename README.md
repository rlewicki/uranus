# Uranus

This project utilize [premake](https://github.com/premake/premake-core) functionality that isn't yet
officially supported. Required changes are visible in [this](https://github.com/premake/premake-core/pull/1252)
pull request.

# How to build

To generate gnu make file use:

```
premake5 gmake --cc=lcc
```

and then in root directory call

```
make
```

