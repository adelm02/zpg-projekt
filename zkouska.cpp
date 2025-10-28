#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <__filesystem/perm_options.h>
#include <cstdlib>
#include <ctime>

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
#include "Light.h"
#include "suzi_smooth.h"
#include "triangle.h"
#include "plain.h"



extern Tranform moveEarth;
extern Tranform moveMoon;

extern Transformation tEarth;
extern Transformation tMoon;
Tranform moveEarth(5.0f, 0.0f, 0.0f);
Tranform moveMoon (6.2f, 0.0f, 0.0f);

Transformation tEarth;
Transformation tMoon;


int main() {

    SceneManager manager;
    Application app(manager);
    if (!app.init(800, 600, "ZPG")) return -1;
    GLFWwindow* window = app.getWindow();

    std::srand(42); // simple deterministic seed for beginner-friendly random


    Shader vertex;
    vertex.createShaderFromFile(GL_VERTEX_SHADER, "shaders/vertex.vert");

    Shader fragment_lambert;
    fragment_lambert.createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/lambert.frag");

    Shader fragment_phong;
    fragment_phong.createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/phong.frag");

    Shader fragment_blinn;
    fragment_blinn.createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/blinn.frag");

    Shader fragC;
    fragC.createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/fragmentColor.frag");

    ShaderProgram programLambert;
    programLambert.addShader(vertex);
    programLambert.addShader(fragment_lambert);
    programLambert.link();

    const int N = 12;
    glm::vec3 lightPos[N];
    glm::vec3 lightColor[N];
    glm::vec3 lightAtten[N];

    for (int i = 0; i < N; ++i) {
        float rx = (std::rand() / (float)RAND_MAX) * 90.0f;
        float rz = (std::rand() / (float)RAND_MAX) * 40.0f;
        float ry = 2.2f + (std::rand() / (float)RAND_MAX) * 1.4f;
        lightPos[i]   = glm::vec3(rx, ry, rz);
        lightColor[i] = glm::vec3(1.00f, 0.90f, 0.60f);
        lightAtten[i] = glm::vec3(1.0f, 0.10f, 1.4f);
    }

    programLambert.useShaderProgram();
    programLambert.SetUniform("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
    programLambert.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    programLambert.SetUniform("objectColor", glm::vec3(0.385f, 0.647f, 0.812f));
    programLambert.update();

    ShaderProgram programPhong;
    programPhong.addShader(vertex);
    programPhong.addShader(fragment_phong);
    programPhong.link();
    programPhong.useShaderProgram();


    programPhong.useShaderProgram();
    programPhong.SetUniform("objectColor", glm::vec3(0.385f, 0.647f, 0.812f));
    programPhong.SetUniform("lightCount", N);
    programPhong.SetUniformArray3("lightPos",   lightPos,   N);
    programPhong.SetUniformArray3("lightColor", lightColor, N);
    programPhong.SetUniformArray3("lightAtten", lightAtten, N);
    programPhong.SetUniform("viewPos", Camera::getInstance()->getCameraPos());

    ShaderProgram programBlinn;
    programBlinn.addShader(vertex);
    programBlinn.addShader(fragment_blinn);
    programBlinn.link();
    programBlinn.useShaderProgram();

    const int NB = 1;
    glm::vec3 blinnPos [NB] = { glm::vec3(2.0, 0, -6) };
    glm::vec3 blinnColor[NB] = { glm::vec3(1.0f, 1.0f, 1.0f) };
    glm::vec3 blinnAtt [NB] = { glm::vec3(1.0f, 0.0f, 0.0f) };

    programBlinn.useShaderProgram();
    programBlinn.SetUniform("lightCount", NB);
    programBlinn.SetUniformArray3("lightPos",   blinnPos,   NB);
    programBlinn.SetUniformArray3("lightColor", blinnColor, NB);
    programBlinn.SetUniformArray3("lightAtten", blinnAtt,   NB);
    programBlinn.SetUniform("viewPos", Camera::getInstance()->getCameraPos());
    programBlinn.SetUniform("objectColor", glm::vec3(0.8f, 0.8f, 0.9f));

    programBlinn.update();

    ShaderProgram programConstant;
    programConstant.addShader(vertex);
    programConstant.addShader(fragC);
    programConstant.link();

    Modell strom;
    strom.loadData(tree, sizeof(tree)/sizeof(float), 6);

    Modell bush;
    bush.loadData(bushes, sizeof(bushes)/sizeof(float), 6);

    Modell koule;
    koule.loadData(sphere, sizeof(sphere)/sizeof(float), 6);

    Modell suzi_smoth;
    suzi_smoth.loadData(suziSmooth, sizeof(suziSmooth)/sizeof(float), 6);

    Modell tria;
    tria.loadData(triangle, sizeof(triangle)/sizeof(float), 6);

    Modell pl;
    pl.loadData(plain, sizeof(plain)/sizeof(float), 6);


    std::vector<Transformation*> fireflyTrans;
    std::vector<DrawableObject*> fireflies;

    for (int i = 0; i < N; ++i) {
        Transformation* tf = new Transformation();
        Scale* sFly = new Scale(0.12f, 0.12f, 0.12f);
        Tranform* mFly = new Tranform(lightPos[i].x, lightPos[i].y, lightPos[i].z); // light position
        tf->addTrans(sFly);
        tf->addTrans(mFly);

        DrawableObject* fly = new DrawableObject(koule, programConstant, *tf, glm::vec3(1.0f));

        fireflyTrans.push_back(tf);
        fireflies.push_back(fly);
    }


    std::vector<Transformation*> transforms;
    std::vector<DrawableObject*> stromy;
    std::vector<DrawableObject*> bushes;


    for (int i = 0; i < 50; i++) {
        float x = (i % 10) * 10.0f;
        float z = (i / 10) * 10.0f;
        float x1 = x + 5.0f;
        float z1 = z + 5.0f;


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

        DrawableObject* stromek = new DrawableObject(strom, programPhong, *t, glm::vec3(0.20f, 0.65f, 0.40f));
        stromy.push_back(stromek);

        DrawableObject* bushi = new DrawableObject(bush, programPhong, *tBush, glm::vec3(0.12f, 0.45f, 0.18f));
        bushes.push_back(bushi);
    }


    Tranform kvpravo (2.f, 0.f, 0.f);
    Tranform kvlevo (-2.f, 0.f, 0.f);
    Tranform kdole (0.f, -2.f, 0.f);
    Tranform hore (0.f, 2.f, 0.f);
    Tranform stred (0.f, 0.f, 0.f);

    Transformation tRight;
    tRight.addTrans(&kvpravo);

    Transformation tLeft;
    tLeft.addTrans(&kvlevo);

    Transformation tUp;
    tUp.addTrans(&hore);

    Transformation tDown;
    tDown.addTrans(&kdole);

    Transformation middle;
    middle.addTrans(&stred);

    Scale zem (50.0f, 1.0f, 30.0f);
    Transformation zemm;
    zemm.addTrans(&zem);
    Tranform groundMove(45.0f, 0.0f, 20.0f);
    zemm.addTrans(&groundMove);

    //před trans jinak se rozbije
    Scale earthScale(0.6f, 0.6f, 0.6f);
    Scale moonScale(0.25f, 0.25f, 0.25f);
    tEarth.addTrans(&earthScale);
    tMoon.addTrans(&moonScale);
    tEarth.addTrans(&moveEarth);
    tMoon.addTrans(&moveMoon);

    DrawableObject* earthObject = new DrawableObject(koule, programPhong, tEarth);
    DrawableObject* moonObject  = new DrawableObject(koule, programPhong, tMoon);


    DrawableObject* sphereRight = new DrawableObject(koule, programPhong, tRight);
    DrawableObject* sphereLeft = new DrawableObject(koule, programPhong, tLeft);
    DrawableObject* sphereUp = new DrawableObject(koule, programPhong, tUp);
    DrawableObject* sphereDown = new DrawableObject(koule, programPhong, tDown);

    DrawableObject* triangle = new DrawableObject(koule, programBlinn, middle);

    DrawableObject* plain = new DrawableObject(pl, programPhong, zemm, glm::vec3(0.27f, 0.25f, 0.15f));

    DrawableObject* sunObject = new DrawableObject(koule, programPhong, middle);


    static Scene scene1;
    static Scene scene2;
    static Scene scene3;
    static Scene scene4;
    static Scene scene5;


    for (auto &st : stromy) {
        scene3.addObject(st);
    }

    for (auto &b : bushes) {
        scene3.addObject(b);
    }


    for (auto* f : fireflies) {
        scene3.addObject(f);
    }

    scene1.addObject(triangle);

    scene4.addObject(earthObject);
    scene4.addObject(moonObject);
    scene4.addObject(sunObject);

    scene2.addObject(sphereRight);
    scene2.addObject(sphereLeft);
    scene2.addObject(sphereUp);
    scene2.addObject(sphereDown);

    scene3.addObject(plain);

    manager.addScene(&scene1);
    manager.addScene(&scene2);
    manager.addScene(&scene3);
    manager.addScene(&scene4);

    app.Run();

    return 0;
}