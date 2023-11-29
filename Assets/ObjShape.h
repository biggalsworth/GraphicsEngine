#pragma once
#include "common.h"

struct moveAnim 
{
    glm::vec3 pos1;
    glm::vec3 pos2;
    glm::vec3 targPos;
};

class ObjShape
{
public:
    // mesh data
    std::vector<VertexData> shapeData;
    const char* fileN = "";
    float currAngle = 0;
    glm::vec3 Pos1;
    glm::vec3 targPos;
    glm::vec3 currPos;
    bool inc;
    std::vector<moveAnim> anims;
    glm::mat4 transform = glm::mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    //std::vector<VertexData> texture;

    ObjShape(std::vector<VertexData> vData)
    {
        this->fileN = "Assets/imageData/CubeImageInf.png";
        this->shapeData = vData;
        inc = true;
        ConfigObjIntoShape();
    }
    void Draw()
    {
        // draw mesh
        glBindVertexArray(vao);
        //glDrawElements(GL_TRIANGLES, shapeData.size(), GL_UNSIGNED_INT, NULL);
        glDrawArrays(GL_TRIANGLES, 0, shapeData.size());
        //glBindVertexArray(0);
    }

    void ConfigLightSource()
    {
        //add light vao
        unsigned int lightCubeVAO;
        glGenVertexArrays(1, &lightCubeVAO);
        glBindVertexArray(lightCubeVAO);

        // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
        glBindVertexArray(0);
    }

private:
    //  render data
    unsigned int vao, vbo;

    void ConfigObjIntoShape()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        //glGenBuffers(1, &ibo);
        //Bind the buffer/vertex array to the vao to allow the shader to use it.
        glBindVertexArray(vao);


        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, shapeData.size() * sizeof(VertexData), shapeData.data(), GL_STATIC_DRAW);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        //colour
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // vertex normals
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
        // vertex texture coords
        //glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, colour));
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)(10 * sizeof(float)));
        glEnableVertexAttribArray(3);


    }

    /*
    void Animate() 
    {
        if (pos1 != targetPos) 
        {

        }
    }*/

};

std::vector<VertexData> LoadOBJ(const char* fileName)
{
    //These vectors will hold the data we read from each line
    //.obj indices start from 1 (and not 0!), so using (1) with a vector
    //adds a dummy value as index 0 of all vertices
    //We could also use a -1 later when putting together the final vertices
    //if you wished and remove the dummy value
    std::vector<glm::vec3> positions(1); //v
    std::vector<glm::vec3> normals(1); //vn
    std::vector<glm::vec2> textureCoords(1); //vt
    std::vector<glm::ivec3> faceIndices(1); //f

    //This vector will hold the vertices which we will build up using the face indices 
    //via the 'f' prefix lines, and be output by this function
    std::vector<VertexData> finalVertices;


    std::ifstream inObjFile(fileName);

    //Setting up variables 
    //We use the temp values to collect data from the read lines before 
    //we create the final vertices from the face indices
    std::stringstream ss;
    std::string line = "";
    std::string prefix = "";
    glm::vec3 tempVector3;
    glm::vec2 tempVector2;
    glm::ivec3 tempiVector3;

    //file open error check, if it cannot find the file, the program will crash
    if (!inObjFile.is_open())
    {
        throw "ERROR - Cannot find obj file!";
    }

    //read one line at a time from the file
    while (std::getline(inObjFile, line))
    {
        //clear the stringstream for the next line
        ss.clear();
        ss.str(line);
        //Look at the prefix - the characters before the first space on the line
        ss >> prefix;


        if (prefix == "v") //it is a vertex position so save this away
        {
            //format = v x y z
            ss >> tempVector3.x >> tempVector3.y >> tempVector3.z;
            positions.emplace_back(tempVector3);
        }
        else if (prefix == "vt") //it is a vertex texture co-ordinate so save this away
        {
            //format = vt u v
            ss >> tempVector2.x >> tempVector2.y;
            textureCoords.emplace_back(tempVector2);
        }
        else if (prefix == "vn") //it is a vertex normal so save this away
        {
            //format = vn x y z
            ss >> tempVector3.x >> tempVector3.y >> tempVector3.z;
            normals.emplace_back(tempVector3);
        }
        else if (prefix == "f") //it is a face so save this away
        {
            //format = f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3
            int tempInt;

            //Slightly longer code than the rest as we need to keep going for 
            //the 3 vertex indices a face is "made up of"
            while (ss >> tempInt)
            {
                tempiVector3.x = tempInt;

                //we need to ignore a / or space so peek() checks that for us
                //if the next character is a / or space, move past it
                if (ss.peek() == '/')
                    ss.ignore(1, '/');
                else if (ss.peek() == ' ')
                    ss.ignore(1, ' ');

                ss >> tempiVector3.y;

                if (ss.peek() == '/')
                    ss.ignore(1, '/');
                else if (ss.peek() == ' ')
                    ss.ignore(1, ' ');

                ss >> tempiVector3.z;

                //Now we have the 3 indices, put this as a ivec3
                faceIndices.emplace_back(tempiVector3);
            }
        }
        else
        {
            //it is something we do not need so ignore it
        }
    }

    for (int i = 1; i < faceIndices.size(); ++i)
    {
        //There is no colour data in the .obj and for this example,
        //assume the Vertex struct/class has both position and colour to fill
        //so this example just adds a magenta colour
        //Change this to suit your needs!
        glm::vec4 colour(1, 0, 1, 1);

        //The creation of your vertex struct/class 
        //In the example, we are not using the normal or texture co-ordinate data
        VertexData vertex{
            {
                positions[faceIndices[i].x].x,
                positions[faceIndices[i].x].y,
                positions[faceIndices[i].x].z
            },
            { colour.r, colour.g, colour.b, colour.a},
            { normals[faceIndices[i].z].x,  normals[faceIndices[i].z].y,  normals[faceIndices[i].z].z },
            { textureCoords[faceIndices[i].y].x, textureCoords[faceIndices[i].y].y }
        };

        //Add this vertex to the vertex vector
        finalVertices.emplace_back(vertex);
    }
    
    //output the vector of vertices filled with data
    return finalVertices;

};