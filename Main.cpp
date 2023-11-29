#pragma once
#include <iostream>
#include <vector>
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"
#include <fstream>
#include "Main.h"
#include "Assets/common.h"
#include "Assets/Camera.h"
#include "Assets/Shader.h"
#include "Assets/ObjShape.h"
#include "Assets/LightObj.h"
#include "Assets/Textures/CubemapTexture.h"
#include "Assets/Textures/Texture.h"
#include <list>

//GLuint vao;
//GLuint vbo;

int windowWidth = 1880;
int windowHeight = 920;
float FOV = 45;
float nearPlane = 0.05;
float farPlane = 100.0f;

bool splashScreen = true;

#pragma region View Matrix Variables

//Define base axis
glm::vec3 targetPos = glm::vec3(0, 0, 0);
glm::vec3 cameraPos = glm::vec3(0, 5, 10);
glm::vec3 upPos = glm::vec3(0, 1, 0);
glm::vec3 right = glm::vec3(1, 0, 0);
glm::mat4 ViewRotateX;

Camera camera(cameraPos, upPos);


//store the original position of the mouse
//this division sets it to the centre of the screen 
float prevX = windowWidth / 2.0f;
float prevY = windowHeight / 2.0f;
#pragma endregion


#pragma region Functions

void showMessage(char message)
{
    std::cout << message << std::endl;
}

void ShaderCompileChecker(GLuint shader)
{
    GLint result = GL_FALSE;
    int logLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    //remove the comments on the code below to make program crash with error
    //if (result == GL_FALSE)
    //throw std::exception("Shader error! Check the console output to see where the error in the shader code is!");
}

//input event

void KeyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //make sure that user can't move during introduction
    if (!splashScreen) 
    {
        camera.Input(key);

    }
    else 
    {
        //stop the splash screen when any input is detected
        splashScreen = false;
    }
    /*
    Old camera movement
    char inputChar = char(key);

    if (inputChar == 'D') 
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cameraPos = rotationMatrix * glm::vec4(cameraPos, 1.0f);

    }
    if (inputChar == 'A')
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cameraPos = rotationMatrix * glm::vec4(cameraPos, 1.0f);
    }
    if(inputChar == 'W')
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-10.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        cameraPos = rotationMatrix * glm::vec4(cameraPos, 1.0f);
    }
    if(inputChar == 'S')
    {
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        cameraPos = rotationMatrix * glm::vec4(cameraPos, 1.0f);
    }
    */
}

void MouseInput(GLFWwindow* window, double XIn, double YIn)
{
    if (!splashScreen)
    {
        float xpos = (float)(XIn);
        float ypos = (float)(YIn);

        /*
        //has mouse been used before
        //we need to store the original value of the x and y position
        if (mouseInputed == false)
        {
            lastX = xpos;
            lastY = ypos;
            mouseInputed = true;
        }*/

        float X = xpos - prevX;
        float Y = prevY - ypos; // reversed since y-coordinates go from bottom to top

        camera.MouseInput(X, Y);

        prevX = xpos;
        prevY = ypos;
    }
}


#pragma endregion


int main(int argc, char** argv)
{

	glfwInit();
	//Define GL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    #pragma region GL window Setup

	    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Ben's Graphics Simulation", NULL, NULL);
	    //GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Ben's Graphics Simulation", glfwGetPrimaryMonitor(), NULL);
    
        if (!window)
        {
            std::cout << "ERROR: No window was created!" << std::endl;
            return -1;
        }

        //assign the new windo to the GL's context

        glfwMakeContextCurrent(window);

        //initiate 
        if (gl3wInit())
        {
            std::cout << "ERROR: Failed to initialize OpenGL" << std::endl;
            return -1;
        }

        //How often the buffer should swap - 1 = do everytime there is a screen update
        glfwSwapInterval(1);

        gl3wProcs.gl.Enable(GL_DEPTH_TEST);

        float aspectRatio = windowWidth / windowHeight;
        float verticalFov = glm::radians(FOV);
        float halfFov = glm::tan(verticalFov / 2.0);

    #pragma endregion

    #pragma region Object setup

    #pragma region shader setup

        #pragma region Vertices

        GLfloat vertices[] =
        {
            -0.5, 0.5,
            -0.5, -0.5,
            0.5, -0.5
        };

        GLfloat vertices2[] =
        {
            -0.85, 0.9,
            0.9, 0.9,
            0.9, -0.85
        };

        //Using 4 vec4s to make a matrix
        /*
        glm::mat4 aMatrix(
            glm::vec4(1, 0, 0, 0),
            glm::vec4(0, 1, 0, 0),
            glm::vec4(0, 0, 1, 0),
            glm::vec4(0, 0, 0, 1));*/


        #pragma region Hard coded shapes

            //skybox vertices
            GLfloat skyboxVertices[] = {
                // positions          
                -1.0f,  1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f, -1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f,
                -1.0f, -1.0f,  1.0f,

                -1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f, -1.0f,
                 1.0f,  1.0f,  1.0f,
                 1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f,  1.0f,
                -1.0f,  1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f, -1.0f,
                 1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f,  1.0f,
                 1.0f, -1.0f,  1.0f
            };

            std::vector<float> skyBoxVerts  =
            {
               -10.0f , -10.0f ,  10.0f,//0
                10.0f , -10.0f ,  10.0f,//1
                10.0f , -10.0f , -10.0f,//2
               -10.0f , -10.0f , -10.0f,//3
               -10.0f ,  10.0f ,  10.0f,//now we do top layer of cube //4
                10.0f ,  10.0f ,  10.0f,//5
                10.0f ,  10.0f , -10.0f,//6
               -10.0f ,  10.0f , -10.0f //7
            };

            //all the points for the cube vertices
            std::vector<VertexData> CubeData{
                { {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f, -0.5f} },//starting furthest bottom corner and going around the bottom vertices//0
                { {1.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f, -0.5f} },//1
                { {1.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f,  0.5f} },//2
                { {0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f,  0.5f} },//3
                { {0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f} },//now we do top layer of cube //4
                { {1.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f} },//5
                { {1.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f} },//6
                { {0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f} }//7
            };
            
            std::vector<VertexData> squareData{
                { {-0.5f,   0, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f}, {0 ,0}},
                { { 0.5f,   0, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f}, {1, 0} },
                { { 0.5f,   0,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f}, {1, 1} },
                { {-0.5f,   0,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f}, {0, 1}}
            };
            
            std::vector<VertexData> splashVerts{
                { {-0.5f, -0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f}, {0 ,0}},
                { { 0.5f, -0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f}, {1, 0} },
                { { 0.5f,  0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f}, {1, 1} },
                { {-0.5f,  0.5,   0}  , {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f}, {0, 1}}
            };

            //std::vector<VertexData> CubeData2{
            //    { {-1.5f, -0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f, -0.5f} },//starting furthest bottom corner and going around the bottom vertices//0
            //    { {-0.5f, -0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f, -0.5f} },//1
            //    { {-0.5f, -0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f, -0.5f,  0.5f} },//2
            //    { {-1.5f, -0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f, -0.5f,  0.5f} },//3
            //    { {-1.5f,  0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f, -0.5f} },//now we do top layer of cube //4
            //    { {-0.5f,  0.5f, -0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f, -0.5f} },//5
            //    { {-0.5f,  0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, { 0.5f,  0.5f,  0.5f} },//6
            //    { {-1.5f,  0.5f,  0.5f, 1}, {1.0f, 0.0f, 0.0f, 1.0f}, {-0.5f,  0.5f,  0.5f} }//7
            //};

            //all the points for the cube vertices
            std::vector<unsigned int> cubeFaces
            {
                //top face
                4,7,6,
                6,5,4,

                //bottom face
                0,1,2,
                2,3,0,

                //front face
                2,3,7,
                7,6,2,

                //back face
                0,1,5,
                5,4,0,


                //right face
                1,2,6,
                6,5,1,

                //left face
                0,3,7,
                7,4,0
            };
            //all the points for the cube vertices
            std::vector<unsigned int> squareFaces
            {
                //top face
                0,3,2,
                2,1,0,
            };


        #pragma endregion

        
        #pragma endregion

        //Create shader programs
        //Create them once so we can use less RAM
        Shader cubeMapShader("Assets/Shaders/Skybox/VertexShader.glsl", "Assets/Shaders/Skybox/FragShader.glsl");
        Shader multiShader("Assets/Shaders/LightVertShader.glsl", "Assets/Shaders/MultiLightFrag.glsl");
        Shader unlit("Assets/Shaders/VertexShader.glsl", "Assets/Shaders/FragShader.glsl");

        multiShader.use();

        #pragma region OLD SHADER PROGRAM
        /*
        const char* vertexShaderCode = NULL;

        const char* fragmentShaderCode = NULL;

        const char* fragmentShaderCodeOther =
        {
            "#version 330 core\n \
            \n \
            layout(location = 0) out vec4 fragmentColour; \n \
            \n \
            void main()\n \
            {\n \
            \n \
                fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n \
            \n \
            } \
            "
        };

    #pragma endregion

        GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShaderId);
        ShaderCompileChecker(vertexShaderId);

        GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShaderId);
        ShaderCompileChecker(fragmentShaderId);

        GLuint otherFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(otherFragmentShaderId, 1, &fragmentShaderCodeOther, NULL);
        glCompileShader(otherFragmentShaderId);
        ShaderCompileChecker(otherFragmentShaderId);

        //As modern OpenGL uses the GPU
        //Store this shader in the GPU
        GLuint programId = glCreateProgram(); //create shader program
        GLuint programId2 = glCreateProgram(); //create shader program

        //for each shader program, attach the shaders
        glAttachShader(programId, vertexShaderId);
        glAttachShader(programId, fragmentShaderId);

        glAttachShader(programId2, vertexShaderId);
        glAttachShader(programId2, otherFragmentShaderId);

        //link the shader to the graphics program to make a functioning shader
        glLinkProgram(programId);
        glLinkProgram(programId2);

        #pragma region OLD VERTEX DRAW METHOD
        /*
        glGenVertexArrays(1, &vao); //This is saved into the global vao variable.
        glBindVertexArray(vao);

        //edit the vertices using matrices
        //GLuint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
        //glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix));

        glGenBuffers(1, &vbo); //This is saved into the global vbo variable.
        //Bind the buffer array, which makes this buffer "active" or current.
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(shapeCollection.size()), shapeCollection.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)0);

        //texture attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec2), (GLvoid*)sizeof(glm::vec3));

    

        //storing new vertex into vao memory slot
        GLuint newVao;
        GLuint newVbo;

        glUseProgram(programId2);

        //Store the vertices for vertices2
        glGenVertexArrays(1, &newVao);
        glBindVertexArray(newVao);

        //apply matrix transform
        //GLuint otherModelMatrixLocation = glGetUniformLocation(programId2, "modelMatrix");
        //glUniformMatrix4fv(otherModelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix2));

        glGenBuffers(1, &newVbo); //This is saved into the global vbo variable.
        //Bind the buffer array, which makes this buffer "active" or current.
        glBindBuffer(GL_ARRAY_BUFFER, newVbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices2, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

        glEnableVertexAttribArray(0);
        */


        #pragma endregion
    
    #pragma endregion

    #pragma region Textures

        Texture texture2("Assets/imageData/TestTexture.png", multiShader);
        Texture rockTexture("Assets/imageData/Rock.png", multiShader);
        Texture IdolTexture("Assets/imageData/Idol/base.png", multiShader);
        Texture leafTexture("Assets/imageData/leaf.jpg", multiShader);
        Texture bark("Assets/imageData/barkDark.png", multiShader);
        Texture floorTexture("Assets/imageData/forestFloor.jpg", multiShader, "Assets/imageData/ForestNormal.png");
        Texture templeText("Assets/imageData/TempleImage.jpg", multiShader, "Assets/imageData/TempleNormal.jpg");
        Texture waterText("Assets/imageData/waterImage.jpg", multiShader);
        Texture SplashText("Assets/imageData/SplashScreenImg.png", multiShader);

        //building skybox texture
        std::vector<const char*> faces
        {
            "Assets/imageData/Cubemap/posx.jpg",
            "Assets/imageData/Cubemap/negx.jpg",
            "Assets/imageData/Cubemap/posy.jpg",
            "Assets/imageData/Cubemap/negy.jpg",
            "Assets/imageData/Cubemap/posz.jpg",
            "Assets/imageData/Cubemap/negz.jpg",
        };
        CubemapT CubemapText(faces, cubeMapShader);


    #pragma endregion

    #pragma region Object Creation

        //import the object once and then reuse them, takes less ram
        std::vector<VertexData> cubeObj = LoadOBJ("Assets/Models/cube1.obj");
        std::vector<VertexData> sphereObj = LoadOBJ("Assets/Models/Sphere.obj");
        std::vector<VertexData> treeObj = LoadOBJ("Assets/Models/Other/tree1.obj");
        std::vector<VertexData> leavesObj = LoadOBJ("Assets/Models/Other/leaves.obj");
        std::vector<VertexData> rockObj = LoadOBJ("Assets/Models/rock.obj");
        std::vector<VertexData> idolObj = LoadOBJ("Assets/Models/Other/idol.obj");
        std::vector<VertexData> floorObj = LoadOBJ("Assets/Models/Other/floorLargeUV.obj");
        std::vector<VertexData> templeObj = LoadOBJ("Assets/Models/Other/Temple.obj");

        //building the shapes that will be drawn using the vertex data loded from the OBJ;
        ObjShape skyBox(cubeObj);
        ObjShape CubeObj(cubeObj);

        //hard coded with indices
        Shape SplashScreen(squareData, squareFaces);
        Shape Water(squareData, squareFaces);

        ObjShape rock(rockObj);
        ObjShape temple(templeObj);

        ObjShape floor(floorObj);

        LightObj lightShape(cubeObj);
        lightShape.lightcolour = { 0.25f, 0.02f, 0.02f };
        LightObj lightShape2(cubeObj);
        LightObj lightShape3(cubeObj);

        //--OPTIMISATION--
        //define all my firefly objects here and change their individual positions here so we can draw them easier in the loop
        //won't have to do individual loops or if statements for each positional change
        ObjShape firefly1(sphereObj);
        firefly1.Pos1 = glm::vec3(-4.1, 0.5, 1);
        firefly1.targPos = glm::vec3(-4.1, 1.0, 1);
        unlit.changeTransform(firefly1.Pos1, glm::vec3(1, 1, 1));
        firefly1.currPos = unlit.currPos;

        ObjShape firefly2(sphereObj);
        firefly2.Pos1 = glm::vec3(-4.25, 0.1, 1);
        firefly2.targPos = glm::vec3(-4.25, 0.75, 1);
        unlit.changeTransform(firefly2.Pos1, glm::vec3(1, 1, 1));
        firefly2.currPos = unlit.currPos;

        ObjShape firefly3(sphereObj);
        firefly3.Pos1 = glm::vec3(-3.9, 0.5, 0.5);
        firefly3.targPos = glm::vec3(-3.9, 1.25, 0.5);
        unlit.changeTransform(firefly3.Pos1, glm::vec3(1, 1, 1));
        firefly3.currPos = unlit.currPos;

        ObjShape firefly4(sphereObj);
        firefly4.Pos1 = glm::vec3(-4, 0.25, 1.25);
        firefly4.targPos = glm::vec3(-4, 0.5, 1.25);
        unlit.changeTransform(firefly4.Pos1, glm::vec3(1, 1, 1));
        firefly4.currPos = unlit.currPos;

        ObjShape firefly5(sphereObj);
        firefly5.Pos1 = glm::vec3(-3.7, 0.75, 0.75);
        firefly5.targPos = glm::vec3(-4.1, 1.25, 0.75);
        unlit.changeTransform(firefly5.Pos1, glm::vec3(1, 1, 1));
        firefly5.currPos = unlit.currPos;


        ObjShape FireFlies[] =
        {
            {firefly1},
            {firefly2},
            {firefly3},
            {firefly4},
            {firefly5}
        };


        ObjShape idol(idolObj);

        //all my trees
        //defining all my trees here so its easier to render them in a for loop later
        ObjShape    tree(treeObj);
        ObjShape    leaves(leavesObj);
        //-- OPTIMISATION -- 
        //when creating a lot of trees, i made the decision to create a array for them to build later
        //i realised that because i put them in an ObjShape array it creates individual objects to draw within that array, meaning i only ever need to define one instance
        //of a tree and leaves object
        //this saves memory being used ot store the other instances
        /*
        ObjShape    tree2(treeObj);
        ObjShape  leaves2(leavesObj);
        ObjShape    tree3(treeObj);
        ObjShape  leaves3(leavesObj);
        ObjShape    tree4(treeObj);
        ObjShape  leaves4(leavesObj);
        ObjShape    tree5(treeObj);
        ObjShape  leaves5(leavesObj);
        ObjShape    tree6(treeObj);
        ObjShape  leaves6(leavesObj);
        ObjShape    tree7(treeObj);
        ObjShape  leaves7(leavesObj);*/

        ObjShape allTrees[] =
        {   //25 lines
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree },
            {tree }
        };
        ObjShape allLeaves[] =
        {
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves },
            {leaves }
        };
        std::vector<glm::vec3> treeLoc =
        {
            {- 4, -0.75 ,- 3},
            {  8, -0.75 ,- 3},

            {- 7, -1.00 ,  0},
            {  7, -0.75 ,  0},

            {-10, -0.75 ,  4},
            { 10, -0.75 ,  4},

            {-15, -1.50 ,-15},
            { 15, -1.50 ,-15},

            { 20, -1.50 ,  5},
            {-20, -1.50 ,  5},

            { 23, -0.75 ,-10},
            {-23, -1.75 ,-10},

            { 35, -0.75 ,  0},
            {-35, -0.75 ,  0},

            { 12, -1.00 , 15},
            {-12, -1.00 , 15},

            { 12, -1.00 ,-20},
            {-12, -1.00 ,-20},

            {  0, -1.00 , 15},
            {- 5, -1.00 , 10},

            { 14, -1.00 , 20},
            {-14, -1.00 , 20},

            { 15, -1.00 , -8},
            {-15, -1.00 , -8},
            
            {  3, -1.00 , -23},
            {- 7, -1.00 , -20},
            
            { 24, -1.00 , -27},
            {-24, -1.00 , -27},

            { 23, -1.00 , 20},
            {-23, -1.00 , 20}
        };



    #pragma endregion


    #pragma endregion

    #pragma region Game Loop

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //detect inputs
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, KeyInput); 
    glfwSetCursorPosCallback(window, MouseInput);


    int frameCount = 0;
    int oldTime = 0;


    while (!glfwWindowShouldClose(window))
    {
        #pragma region setting up matrices and shader cameras

        //Camera Co-ords
        /*
        glm::vec3 forwardDir = glm::normalize(cameraPos - targetPos);
        glm::vec3 side = glm::normalize(glm::cross(upPos, forwardDir));
        glm::vec3 newUp = glm::normalize(glm::cross(forwardDir, side));


        viewMatrix = {
            side.x, side.y, side.z, -glm::dot(side, cameraPos),
            newUp.x, newUp.y, newUp.z, -glm::dot(newUp, cameraPos),
            forwardDir.x, forwardDir.y, forwardDir.z, -glm::dot(forwardDir, cameraPos),
            0, 0, 0, 1};*/


        glm::mat4 modelMatrix(
            0.25, 0, 0, 0.25,
            0, 0.25, 0, 0.5,
            0, 0, 0.25, 0,
            0, 0, 0, 1);

        glm::mat4 projMatrix(
            (float)1 / (halfFov * aspectRatio), 0, 0, 0,
            0, (float)1 / halfFov, 0, 0,
            0, 0, (nearPlane + farPlane) / (nearPlane - farPlane), -1,
            0, 0, (2.0f * nearPlane * farPlane) / (nearPlane - farPlane), 0);

        glm::mat4 viewMatrix = camera.GetViewMatrix();


        #pragma region OLD CAMERA BUILD

                //Select collection of vertices we want to draw and request to render them
                //Bind the vertex array we created earlier to select it with its vertex data
                //glUseProgram(programId);
                /*
                glUseProgram(shader1.programId);// select which shader program the matrices will be used for

                GLuint modelMatrixLocation = glGetUniformLocation(shader1.programId, "modelMatrix");
                glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix));

                GLuint viewMatrixLocation = glGetUniformLocation(shader1.programId, "viewMatrix");
                glUniformMatrix4fv(viewMatrixLocation, 1, GL_TRUE, glm::value_ptr(viewMatrix));

                GLuint projectionMatrixLocation = glGetUniformLocation(shader1.programId, "ProjectionMatrix");
                glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));


                glUseProgram(shader2.programId);

                GLuint otherModelMatrixLocation = glGetUniformLocation(shader2.programId, "modelMatrix");
                glUniformMatrix4fv(otherModelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix2));

                GLuint viewMatrixLocation2 = glGetUniformLocation(shader2.programId, "viewMatrix");
                glUniformMatrix4fv(viewMatrixLocation2, 1, GL_TRUE, glm::value_ptr(viewMatrix));

                GLuint projectionMatrixLocation2 = glGetUniformLocation(shader2.programId, "ProjectionMatrix");
                glUniformMatrix4fv(projectionMatrixLocation2, 1, GL_FALSE, glm::value_ptr(projMatrix));*/

        #pragma endregion

        //build camera using shader
        multiShader.buildCam(modelMatrix, viewMatrix, projMatrix);
        unlit.buildCam(modelMatrix, viewMatrix, projMatrix);
        cubeMapShader.buildCam(modelMatrix, viewMatrix, projMatrix);

        #pragma endregion

        //clear the framebuffer/window to a background colour
        const float backgroundColour[] = { 0.2, 0.2, 0.2, 0.0f };
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearBufferfv(GL_COLOR, 0, backgroundColour);

        glfwPollEvents();

        int currentTime = glfwGetTime();
        int deltaTime = currentTime - oldTime;
        frameCount++;

        if ((currentTime - oldTime) > 1.0f)
        {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            oldTime = currentTime;
        }

        #pragma region Drawing Objects
        

        #pragma region Lighting Objects

        //these now control where the lights are
        glm::vec3 sunVec = { 0, -1, 5 };
        glm::vec3 lightVec = { 0.25f, 0.5f, -0.25f };
        glm::vec3 lightVec2 = { -2.0f, 1.25f, -1.0f };
        glm::vec3 lightVec3 = { 0, 2.0f, -9.5 };

        glm::vec3 spotLightPos = { 0, 2.0f, -3 };
        glm::vec3 spotlightDir = { 0, -1, -5};

        glm::vec3 firefly1vec = { -3.8, 0.5, 1 };

        glm::vec3 lights[] = { sunVec, lightVec, lightVec2, lightVec3, firefly1vec };

        glm::vec3 objColour = { 1.0f, 1.0f, 1.0f };
        glm::vec3 lightCol = { 0.25f, 0.25f, 0.25f };

        glm::vec3* pos = new glm::vec3();
        glm::vec3* scale = new glm::vec3();


        texture2.Use();
        //light models
        unlit.use();

        //Batch drawing
        //drawing all my light objects here as they use the same shader so it is easier to process them.
        //sun
        //indices cube
        *pos = glm::vec3(0, 2, -2);
        *scale = glm::vec3(1, 0.5f, 1);

        glm::vec3 SunColour = { 1.0f, 1.0f, 1.0f };
        unlit.assignVec3("objectColour", SunColour);
        unlit.changeTransform(sunVec, *scale, 60, glm::vec3(1, 0, 0));
        //Sun.Draw();

        objColour = { 0.6f, 0, 0 };
        unlit.assignVec3("objectColour", lightShape.lightcolour);
        unlit.changeTransform(lightVec, glm::vec3(0.1, 0.1, 0.1));
        //changing colour of a light
        if (lightShape.changeColour) 
        {
            //if you're allowed to change colour, go more green
            lightShape.lightcolour.y += 0.001;
            //if this light colour has gone far enough, begin to go back
            if (lightShape.lightcolour.y >= 0.5f) 
            {
                lightShape.changeColour = false;
            }
        }
        else 
        {
            //become less green
            lightShape.lightcolour.y -= 0.001;
            if (lightShape.lightcolour.y <= 0.02)
            {
                //once you're back to original, start increasing again
                lightShape.changeColour = true;
            }
        }
        lightShape.Draw();

        objColour = { 1.0f, 1.0f, 1.0f };
        unlit.assignVec3("objectColour", objColour);
        unlit.changeTransform(lightVec2, glm::vec3(0.1, 0.1, 0.1));
        lightShape2.Draw();

        //idol light
        //this light source is put inside an object drawn in the main loop, no need to draws
        unlit.changeTransform(lightVec3, glm::vec3(1, 1, 1));

        #pragma endregion

        //fire flies are here because this also uses the unlit shader, so i don't have to keep swapping shaders later
        //gave a more stable fps
        #pragma region fireflies

        * scale = glm::vec3(0.01, 0.01, 0.01);
        //*pos = glm::vec3(-4.1, 0.5, 1);
        objColour = { 0.8f, 0.9f, 0.0f };
        unlit.assignVec3("objectColour", objColour);
        //unlit.changeTransform(*pos, *scale);
        for (int i = 0; i <= (sizeof(FireFlies) / sizeof(*FireFlies)); i++)
        {
            if (FireFlies[i].inc)
            {
                if ((FireFlies[i].currPos.y + 0.02) > FireFlies[i].targPos.y)
                {
                    FireFlies[i].inc = false;
                }
                else
                {
                    FireFlies[i].currPos.y += 0.02;
                    //firefly1.Draw();
                }
            }
            if (!FireFlies[i].inc)
            {
                if ((FireFlies[i].currPos.y - 0.02) < FireFlies[i].Pos1.y)
                {
                    FireFlies[i].inc = true;
                }
                else
                {
                    FireFlies[i].currPos.y -= 0.02;
                    //firefly1.Draw();
                }
            }
            unlit.changeTransform(FireFlies[i].currPos, *scale);
            FireFlies[i].Draw();
        }
        //old loop
        /*
        *pos = glm::vec3(-3.9, 0.5, 1);
        unlit.changeTransform(*pos, *scale);
        firefly2.Draw();
        *pos = glm::vec3(-4.0, 0.5, 1.1);
        unlit.changeTransform(*pos, *scale);
        firefly3.Draw();
        *pos = glm::vec3(-4.2, 0.5, 1.2);
        unlit.changeTransform(*pos, *scale);
        firefly4.Draw();
        *pos = glm::vec3(-3.8, 0.5, 0.9);
        unlit.changeTransform(*pos, *scale);
        firefly5.Draw();*/
        #pragma endregion


        //use the shader for multiple lightsources
        //change the shader during the loop so we dont have to redifine the shader loads
        multiShader.use();

        #pragma region defining lights

        //assigning the different light points
        float* lightStrength = new float(0.1f); //higher num = lower light
        float* falloffStrength = new float(0.5f); //higher num = smaller the reach of the light

        glm::vec3*  ambient = new glm::vec3(0.5, 0.5, 0.5); //lower value = less light reflected = darker shape
        glm::vec3*   diffuse = new glm::vec3(0.5, 0.5, 0.5);
        glm::vec3* specular = new glm::vec3(0.7, 0.7, 0.1);
        float* shininess = new float(32);

        //assiging the default material values for this shader.
        multiShader.assignVec3("material.ambient",  *ambient  );
        multiShader.assignVec3("material.diffuse",  *diffuse  );
        multiShader.assignVec3("material.specular", *specular );
        multiShader.setFloat("material.shininess",*shininess);

        //defining light sources
        multiShader.assignVec3("pointLights[0].position", lights[1]);
        multiShader.assignVec3("pointLights[0].lightColour", glm::vec3(0,0,0));//lightShape.lightcolour);
        multiShader.setFloat("pointLights[0].constant", 0.01f);
        multiShader.setFloat("pointLights[0].linear", *falloffStrength);
        multiShader.assignVec3("pointLights[0].ambient",   *ambient);
        multiShader.assignVec3("pointLights[0].diffuse",    *diffuse);
        multiShader.assignVec3("pointLights[0].specular", lightShape.lightcolour);
        //light 2
        //lightCol = { 0.75f, 0.75f, 0.75f };
        lightShape2.lightcolour = { 0.75f, 0.75f, 0.75f };
        *falloffStrength = 1.0f; //higher num = smaller the reach of the light

        *ambient = { 0.1,0.1,0.1 };
        multiShader.assignVec3("pointLights[1].position", lights[2]);
        multiShader.assignVec3("pointLights[1].lightColour", lightShape2.lightcolour);
        multiShader.setFloat("pointLights[1].constant", 1.0f);
        multiShader.setFloat("pointLights[1].linear", *falloffStrength);
        multiShader.setFloat("pointLights[1].quadratic", *lightStrength);
        multiShader.assignVec3("pointLights[1].ambient", *ambient);
        multiShader.assignVec3("pointLights[1].diffuse", *diffuse);
        multiShader.assignVec3("pointLights[1].specular",*specular);
        *ambient = { 0.5, 0.5, 0.5 };

        //idol light
        *lightStrength = 0.01f;
        *falloffStrength = 0.01f;
        multiShader.assignVec3("pointLights[2].position", lights[3]);
        multiShader.assignVec3("pointLights[2].lightColour", glm::vec3(0.5, 0.4, 0));
        multiShader.setFloat  ("pointLights[2].constant", 1.0f);
        multiShader.setFloat  ("pointLights[2].linear", *falloffStrength);
        multiShader.setFloat  ("pointLights[2].quadratic", *lightStrength);
        multiShader.assignVec3("pointLights[2].ambient", *ambient);
        multiShader.assignVec3("pointLights[2].diffuse", *diffuse);
        multiShader.assignVec3("pointLights[2].specular",*specular);
        
        //fireflies
        lightCol = { 0.7f, 0.8f, 0.0f };
        multiShader.assignVec3("pointLights[3].position", lights[4]);
        multiShader.assignVec3("pointLights[3].lightColour", glm::vec3(0.9, 0.8, 0));
        multiShader.setFloat  ("pointLights[3].constant", 1.0f);
        multiShader.setFloat  ("pointLights[3].linear", *falloffStrength);
        multiShader.setFloat  ("pointLights[3].quadratic", *lightStrength);
        multiShader.assignVec3("pointLights[3].ambient", *ambient);
        multiShader.assignVec3("pointLights[3].diffuse", *diffuse);
        multiShader.assignVec3("pointLights[3].specular",*specular);

        //spotlight
        lightCol = { 0.7f, 0.7f, 0.7f };
        *ambient = { 0.5f, 0.5f, 0.5f};
        *diffuse = *ambient;
        *specular = {0.75f, 0.75f, 0.75f};
        multiShader.assignVec3("spotLights[0].position", spotLightPos);
        multiShader.assignVec3("spotLights[0].direction", spotlightDir);
        multiShader.assignVec3("spotLights[0].ambient", *ambient);
        multiShader.assignVec3("spotLights[0].diffuse", *diffuse);
        multiShader.assignVec3("spotLights[0].specular", *specular);
        multiShader.setFloat  ("spotLights[0].constant", 1.0f);
        multiShader.setFloat  ("spotLights[0].linear", 0.09f);
        multiShader.setFloat  ("spotLights[0].quadratic", 0.032f);
        multiShader.setFloat  ("spotLights[0].cutOff", glm::cos(glm::radians(50.0f)));
        multiShader.setFloat  ("spotLights[0].outCutOff", glm::cos(glm::radians(55.0f)));
        

        // directional light
        multiShader.assignVec3("directional.lightColour", 1.0f, 1.0f, 1.0f);
        multiShader.assignVec3("directional.direction", sunVec);
        multiShader.assignVec3("directional.ambient", 0.5f, 0.5f, 0.5f);
        multiShader.assignVec3("directional.diffuse", 0.25f, 0.25f, 0.25f);
        multiShader.assignVec3("directional.specular", 0.25, 0.25, 0.25);


        delete lightStrength;
        delete falloffStrength;
        delete ambient;
        delete specular;
        delete diffuse;

        #pragma endregion

        //creating the models
        objColour = { 1.0f, 1.0f, 1.0f };


        //if the splashcreen should be shown, show it and skip the rest of the loop
        if (splashScreen)
        {
            multiShader.use();
            SplashText.Use();
            multiShader.assignVec3("objectColour", { 1,1,1 });
            multiShader.assignVec3("lightCol", { 1,1,1 });
            multiShader.assignVec3("viewPos", camera.position);
            multiShader.changeTransform({ 0, camera.position.y, camera.position.z - 1.75 }, glm::vec3(1.5, 1.5, 1.5), 90, glm::vec3(1, 0, 0));
            SplashScreen.Draw();
            glfwSwapBuffers(window);
            continue;
        }

        //platform
        multiShader.setFloat("material.shininess", 32);
        floorTexture.Use();
        *pos    = glm::vec3(0   ,   -1    , 0     );
        *scale  = glm::vec3(1   ,   1     , 1     );
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        multiShader.changeTransform(*pos, *scale);
        floor.Draw();
        multiShader.setFloat("material.shininess", 64);


        //large cube
        *pos    = glm::vec3(-1      , 0     , -1    );
        *scale  = glm::vec3(0.75f   , 0.75f , 0.75f );    
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        multiShader.changeTransform(*pos, *scale);
        //texture2.Use();
        CubeObj.Draw();

        //rockTexture.Use();
        //Rock cube
        *pos = glm::vec3(4.2f, 0.75, -0.5f);
        *scale = glm::vec3(0.5f, 0.5f, 0.5f);
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        multiShader.changeTransform(*pos, *scale);
        rock.Draw();



        #pragma region trees

        //--OPTIMISATION--
        //--make trees and locations before window loop is started --
        //--this changes frames from ~80 to 280 per second--
        *pos = glm::vec3(4, -0.5, -3);
        *scale = glm::vec3(1, 1, 1);
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position); 
        for (int i = 0; i < treeLoc.size(); i++)
        {
            //Grab the location for the trees
            *pos = treeLoc[i];
            bark.Use();
            multiShader.changeTransform(*pos, *scale);
            allTrees[i].Draw();

            *pos = *pos + glm::vec3(0, 2, 0);
            //multiShader.changeTransform(*pos, *scale);
            //swap to leaf texture
            leafTexture.Use();
            allLeaves[i].Draw();
        };
        //slower
        /*
        tree.Draw();
        glm::vec3 leafPos = *pos + glm::vec3(0, 2, 0);
        leafTexture.Use();
        leaves.Draw();

        bark.Use();
        *pos = glm::vec3(-4, -0.5, -3);
        multiShader.changeTransform(*pos, *scale);
        tree2.Draw();
        leafPos = *pos + glm::vec3(0, 2, 0);
        leafTexture.Use();
        leaves2.Draw();

        bark.Use();
        *pos = glm::vec3(-7, -0.5, 0);
        multiShader.changeTransform(*pos, *scale);
        tree3.Draw();
        leafPos = *pos + glm::vec3(0, 2, 0);
        leafTexture.Use();
        leaves3.Draw();

        bark.Use();
        *pos = glm::vec3(7, -0.5, 0);
        multiShader.changeTransform(*pos, *scale);
        tree4.Draw();
        leafPos = *pos + glm::vec3(0, 2, 0);
        leafTexture.Use();
        leaves4.Draw();
        //}*/

        #pragma endregion
        
        //temple
        templeText.Use();
        *pos = glm::vec3(0, 0.75, -10);
        *scale = glm::vec3(1, 1, 1);
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        multiShader.changeTransform(*pos, *scale, 90, glm::vec3(1, 0, 0));
        temple.Draw();

        //center spinning object
        IdolTexture.Use();
        *pos = glm::vec3(0, 2.0f, -10);
        *scale = glm::vec3(0.25, 0.25, 0.25);
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        //rotating the object
        multiShader.changeTransform(*pos, *scale, idol.currAngle, glm::vec3(0, 1, 0));
        //increase its currently stored rotation
        idol.currAngle += 2;
        if (idol.currAngle == 360)
        {
            idol.currAngle = 0;
        }
        idol.Draw();


        //water
        waterText.Use();
        *pos = glm::vec3(-4, -0.1, 1);
        //*pos = glm::vec3(0, 1, 0);
        *scale = glm::vec3(4, 2, 4);
        multiShader.assignVec3("objectColour", objColour);
        multiShader.assignVec3("lightCol", lightCol);
        multiShader.assignVec3("viewPos", camera.position);
        multiShader.setFloat("offset", Water.currOffset);
        Water.currOffset += 0.001;
        multiShader.setFloat("material.shininess", 8);

        multiShader.changeTransform(*pos, *scale);
        Water.Draw();
        multiShader.setFloat("material.shininess", *shininess);
        multiShader.setFloat("offset", 0);

        *pos = glm::vec3(0, 5, 0);
        *scale = glm::vec3(40, 40, 40);
        #pragma region Skybox
        glDepthMask(GL_FALSE);
        CubemapText.Use();
        cubeMapShader.use();
        cubeMapShader.setInt("skybox", 0);
        cubeMapShader.changeTransform(*pos, *scale);
        skyBox.Draw();
        glDepthMask(GL_TRUE);
        #pragma endregion



        //finished drawing, clean up pointers
        delete pos;
        delete scale;


        #pragma region Old triangle


        /*
        //glBindVertexArray(vao);
        //apply matrix transform to this shape
        GLuint modelMatrixLocation = glGetUniformLocation(programId, "modelMatrix");
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix));

        GLuint viewMatrixLocation = glGetUniformLocation(programId, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_TRUE, glm::value_ptr(viewMatrix));

        GLuint projectionMatrixLocation = glGetUniformLocation(programId, "ProjectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
        //send vertex data to GL to be rendered
        glLineWidth(5);
        //glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        const std::vector<VertexData> shapeCollection {
            { {-0.9f, 0.85f, 0.0f}, {1.0f, 0.0f, 0.0f} },
            { {-0.9f, -0.9f, 0.0f}, {1.0f, 0.0f, 0.0f} },
            { {0.85f, -0.9f, 0.0f}, {1.0f, 0.0f, 0.0f} }
        };


        const std::vector<VertexData> shapeCollection2{
            { {-0.85f, 0.9f, 0.0f}, {1.0f, 0.0f, 0.0f} },
            { {0.9f, 0.9f, 0.0f}, {1.0f, 0.0f, 0.0f} },
            { {0.9f, -0.85f, 0.0f}, {1.0f, 0.0f, 0.0f} }
        };

        Shape shape1(shapeCollection);
        Shape shape2(shapeCollection2);

        shape1.Draw();

        glUseProgram(programId2); //change which shader will be drawn
        //glBindVertexArray(newVao);
        GLuint otherModelMatrixLocation = glGetUniformLocation(programId2, "modelMatrix");
        glUniformMatrix4fv(otherModelMatrixLocation, 1, GL_TRUE, glm::value_ptr(modelMatrix2));

        GLuint viewMatrixLocation2 = glGetUniformLocation(programId2, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation2, 1, GL_TRUE, glm::value_ptr(viewMatrix));

        GLuint projectionMatrixLocation2 = glGetUniformLocation(programId2, "ProjectionMatrix");
        glUniformMatrix4fv(projectionMatrixLocation2, 1, GL_FALSE, glm::value_ptr(projMatrix));

        //glDrawArrays(GL_TRIANGLES, 0, 3);


        shape2.Draw();*/
        #pragma endregion

        #pragma endregion

        //Redraw the contents of the window and present it to the user ("paints the screen")
        glfwSwapBuffers(window);

    }

    #pragma endregion

    //CLEANUP

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
