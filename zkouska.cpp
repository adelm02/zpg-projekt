#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <__filesystem/perm_options.h>

#include "Shader.h"
#include "ShaderProgram.h"
#include "Application.h"
#include "Modell.h"
#include "Scene.h"
#include "Transformation.h"
#include "DrawableObject.h"
#include "Rotate.h"
#include "Scale.h"
#include "Tranform.h"
#include "AbstractTrans.h"
#include "SceneManager.h"
#include "tree.h"
#include "bushes.h"


const float points[] = {
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f
};


const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;\n"
"uniform mat4 model;\n"
"void main() {\n"
"    gl_Position = model * vec4(vp, 1.0);\n"
"}";

const char* fragment_shader =
"#version 330\n"
"out vec4 fragColor;\n"
"void main() {\n"
"    fragColor = vec4(0.5, 0.0, 0.5, 1.0);\n"
"}";

const char* vertex_shader2 =
"#version 330\n"
"layout(location=0) in vec3 vp;\n"
"layout(location=1) in vec3 vn;\n"
"out vec3 fragColor;\n"
"uniform mat4 model;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"void main() {\n"
"    fragColor = vn;\n"
"    gl_Position = projectionMatrix * viewMatrix * model * vec4(vp, 1.0);\n"
"}";

const char* fragment_shader2 =
"#version 330\n"
"in vec3 fragColor;\n"
"out vec4 frag_color;\n"
"uniform vec3 viewPos;\n"
"void main() {\n"
"    frag_color = vec4(fragColor, 1.0);\n"
"}";



int main() {

    SceneManager manager;
    Application app(manager);
    if (!app.init(800, 600, "ZPG")) return -1;
    GLFWwindow* window = app.getWindow();

    Shader vertex2(vertex_shader2, "vertex");
    vertex2.Compile();

    Shader fragment2(fragment_shader2, "fragment");
    fragment2.Compile();

    ShaderProgram colorProgram;
    colorProgram.addShader(vertex2);
    colorProgram.addShader(fragment2);
    colorProgram.link();

    Modell strom;
    strom.loadData(tree, sizeof(tree)/sizeof(float), 6);

    Modell bush;
    bush.loadData(bushes, sizeof(bushes)/sizeof(float), 6);

    std::vector<Transformation*> transforms;
    std::vector<DrawableObject*> stromy;
    std::vector<DrawableObject*> bushes;


    for (int i = 0; i < 50; i++) {
        float x = (i % 10) * 10.0f;
        float z = (i / 10) * 10.0f;
        float x1 = ((i % 13) * 10.0f) + 5.0f;
        float z1 = ((i / 13) * 10.0f) + 5.0f;


        Transformation* t = new Transformation();
        Tranform* move = new Tranform(x, 0.0f, z);
        t->addTrans(move);
        transforms.push_back(t);

        Transformation* tBush = new Transformation();
        Tranform* moveBush = new Tranform(x1, 0.0f, z1);
        Scale* s = new Scale(3.f, 3.f, 3.f);
        tBush->addTrans(s);
        tBush->addTrans(moveBush);

        transforms.push_back(tBush);

        DrawableObject* stromek = new DrawableObject(strom, colorProgram, *t);
        stromy.push_back(stromek);

        DrawableObject* bushi = new DrawableObject(bush, colorProgram, *tBush);
        bushes.push_back(bushi);
    }

    static Scene scene1;


    for (auto &st : stromy) {
        scene1.addObject(st);
    }


    for (auto &b : bushes) {
        scene1.addObject(b);
    }

    manager.addScene(&scene1);

    app.Run();

    return 0;
}