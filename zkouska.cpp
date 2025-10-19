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
    programPhong.SetUniform("lightPos", glm::vec3(0.0f, 0.0f, 0.0f));
    programPhong.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    programPhong.SetUniform("objectColor", glm::vec3(0.385f, 0.647f, 0.812f));
    programPhong.SetUniform("viewPos", Camera::getInstance()->getCameraPos());
    programPhong.update();


    ShaderProgram programBlinn;
    programBlinn.addShader(vertex);
    programBlinn.addShader(fragment_blinn);
    programBlinn.link();

    programBlinn.useShaderProgram();
    programBlinn.SetUniform("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
    programBlinn.SetUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    programBlinn.SetUniform("objectColor", glm::vec3(0.385f, 0.647f, 0.812f));
    programBlinn.SetUniform("viewPos", Camera::getInstance()->getCameraPos());
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

        DrawableObject* stromek = new DrawableObject(strom, programLambert, *t);
        stromy.push_back(stromek);

        DrawableObject* bushi = new DrawableObject(bush, programLambert, *tBush);
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

    Scale zem (200.0f, 1.0f, 200.0f);
    Transformation zemm;
    zemm.addTrans(&zem);

    // scale Earth and Moon (apply before translation so scale isn't affected by move)
    Scale earthScale(0.6f, 0.6f, 0.6f);
    Scale moonScale(0.25f, 0.25f, 0.25f);
    tEarth.addTrans(&earthScale);
    tMoon.addTrans(&moonScale);
    tEarth.addTrans(&moveEarth);
    tMoon.addTrans(&moveMoon);

    DrawableObject* earthObject = new DrawableObject(koule, programPhong, tEarth);
    DrawableObject* moonObject  = new DrawableObject(koule, programPhong, tMoon);


    DrawableObject* sphereRight = new DrawableObject(koule, programBlinn, tRight);
    DrawableObject* sphereLeft = new DrawableObject(koule, programBlinn, tLeft);
    DrawableObject* sphereUp = new DrawableObject(koule, programBlinn, tUp);
    DrawableObject* sphereDown = new DrawableObject(koule, programBlinn, tDown);

    DrawableObject* suzi = new DrawableObject(suzi_smoth, programBlinn, middle);

    DrawableObject* triangle = new DrawableObject(tria, programConstant, middle);

    DrawableObject* plain = new DrawableObject(pl, programLambert, zemm);

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

    scene1.addObject(triangle);

    scene4.addObject(earthObject);
    scene4.addObject(moonObject);
    scene4.addObject(sunObject);

    scene2.addObject(sphereRight);
    scene2.addObject(sphereLeft);
    scene2.addObject(sphereUp);
    scene2.addObject(sphereDown);

    scene5.addObject(suzi);

    scene3.addObject(plain);

    manager.addScene(&scene1);
    manager.addScene(&scene2);
    manager.addScene(&scene3);
    manager.addScene(&scene4);

    app.Run();

    return 0;
}