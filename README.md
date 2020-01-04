#编译器与基本的函式库
`sudo apt-get install build-essential`
#OpenGL Library
`sudo apt-get install libgl1-mesa-dev`
#OpenGL Utilities 是一组建构于 OpenGL Library 之上的工具组，提供许多很方便的函式，使 OpenGL 更强大且更容易使用。
`sudo apt-get install libglu1-mesa-dev`
#OpenGL Utility Toolkit 是建立在 OpenGL Utilities 上面的工具箱，除了强化了 OpenGL Utilities 的不足之外，也增加了 OpenGL 对于视窗介面支援。
`sudo apt-get install freeglut3-dev`

```
https://blog.csdn.net/qq_38228254/article/details/78521155
gcc -o test main.cpp -lGL -lGLU -lglut
```