# Cookie Town

## Introduction

**Cookie Town**  is an interactive app/game that we created using OpenGL with c++, where the user will be able to
move freely around a 3D model map. The atmosphere of the map can also change from day to night and vice versa. 
Every time you change the time, music will be playing. The user will 
have a flashlight by default with which they can shine while traveling around the map.

> ## How to use the app
>
>  - You can move with the typical keys(A W S D).
>  - You can change to night time with key 1, and you change day time with key 2.
>  - To stop the program press the esc key.

## How It is made

We created this project by uploading a 3D model with the help of a tutorial called [Modern Opengl 3.0++](https://www.youtube.com/playlist?list=PLRtjMdoYXLf6zUMDJVRZYV-6g6n62vet8) created by [Sonar Systems](https://github.com/SonarSystems).

We also take into consideration the collisions on the map, more precisely the edges. For this we create six variables, two for each of the x, y and z axes. These variables will help us establish the position in which the camera/user does not exceed the limit of the map.

### Notes:

- Cookie Town was created for a university project
- If the model does not load or you have problems with the assimp library, it could be because you do not have the model well integrated. 
This must have a specific path as in the following instruction:

`Model ourModel("res/mapa/mapa.obj");`

- You can download the model [here](https://drive.google.com/file/d/1l6U4HSNFTfZUNLIZZkNClYQBd9BppgJG/view?usp=sharing)

## Build with

- c++
- [OpenGL](https://www.opengl.org/)
    - [GLFW](https://www.glfw.org/)
    - [GLM](https://glm.g-truc.net/0.9.9/index.html)
    - [ASSIMP](https://github.com/assimp/assimp)
    - [SOIL2](https://github.com/SpartanJ/SOIL2)


## Authors

- **Ilich Mendez** - [Ilichmendez](https://github.com/Ilichmendez)

- **Obed Cortez**  - [saicob](https://github.com/saicob)
  
- **Nohelia Vivas** - [noeangeles](https://github.com/noeangeles)
  
- **Erick Gonzalez** - [Drafttw30](https://github.com/Drafttw30)
