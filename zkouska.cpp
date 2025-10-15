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
#include "sphere.h"
#include "bushes.h"



int main() {

    SceneManager manager;
    Application app(manager);
    if (!app.init(800, 600, "ZPG")) return -1;
    GLFWwindow* window = app.getWindow();


    Shader vertex;
    vertex.createShaderFromFile(GL_VERTEX_SHADER, "shaders/vertex.glsl");

    Shader fragment;
    fragment.createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/fragment.glsl");

    ShaderProgram program;
    program.addShader(vertex);
    program.addShader(fragment);
    program.link();

    Modell strom;
    strom.loadData(tree, sizeof(tree)/sizeof(float), 6);

    Modell bush;
    bush.loadData(bushes, sizeof(bushes)/sizeof(float), 6);

    Modell koule;
    koule.loadData(sphere, sizeof(sphere)/sizeof(float), 6);

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

        DrawableObject* stromek = new DrawableObject(strom, program, *t);
        stromy.push_back(stromek);

        DrawableObject* bushi = new DrawableObject(bush, program, *tBush);
        bushes.push_back(bushi);
    }


    Tranform kvpravo (2.f, 0.f, 0.f);
    Tranform kvlevo (-2.f, 0.f, 0.f);
    Tranform kdole (0.f, -2.f, 0.f);
    Tranform hore (0.f, 2.f, 0.f);

    Transformation tRight;
    tRight.addTrans(&kvpravo);

    Transformation tLeft;
    tLeft.addTrans(&kvlevo);

    Transformation tUp;
    tUp.addTrans(&hore);

    Transformation tDown;
    tDown.addTrans(&kdole);


    DrawableObject* sphereRight = new DrawableObject(koule, program, tRight);
    DrawableObject* sphereLeft = new DrawableObject(koule, program, tLeft);
    DrawableObject* sphereUp = new DrawableObject(koule, program, tUp);
    DrawableObject* sphereDown = new DrawableObject(koule, program, tDown);


    static Scene scene1;
    static Scene scene2;


    for (auto &st : stromy) {
        scene1.addObject(st);
    }


    for (auto &b : bushes) {
        scene1.addObject(b);
    }

    scene2.addObject(sphereRight);
    scene2.addObject(sphereLeft);
    scene2.addObject(sphereUp);
    scene2.addObject(sphereDown);

    manager.addScene(&scene1);
    manager.addScene(&scene2);

    app.Run();

    return 0;
}