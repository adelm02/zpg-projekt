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
#include "sphere.h"
#include "bushes.h"
#include "tree.h"
#include "suzi_smooth.h"


const float points[] = {
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,

    -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f
};

const float points2[] = {
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

const float points3[] = {
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
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
"layout(location=1) in vec3 vc;\n"
"out vec3 fragColor;\n"
"uniform mat4 model;\n"
"void main() {\n"
"    fragColor = vc;\n"
"    gl_Position = model * vec4(vp, 1.0);\n"
"}";

const char* fragment_shader2 =
"#version 330\n"
"in vec3 fragColor;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"    frag_color = vec4(fragColor, 1.0);\n"
"}";



int main() {


    SceneManager manager;
    Application app(manager);
    if (!app.init(800, 600, "ZPG")) return -1;
    GLFWwindow* window = app.getWindow();

    Modell ctverec;
    ctverec.loadData(points, sizeof(points)/sizeof(float), 6);

    Modell trojuhelnik;
    trojuhelnik.loadData(points2, sizeof(points2)/sizeof(float), 3);

    Modell trojuhelnik2;
    trojuhelnik2.loadData(points3, sizeof(points3)/sizeof(float), 6);

    Modell koule;
    koule.loadData(sphere, sizeof(sphere)/sizeof(float), 6);

    Modell křok;
    křok.loadData(bushes, sizeof(bushes)/sizeof(float), 6);

    Modell suzi;
    suzi.loadData(suziSmooth, sizeof(suziSmooth)/sizeof(float), 6);

    Modell stromek;
    stromek.loadData(tree, sizeof(tree)/sizeof(float), 6);

    Shader vertex(vertex_shader, "vertex");
    vertex.Compile();

    Shader fragment(fragment_shader, "fragment");
    fragment.Compile();

    ShaderProgram purpleProgram;
    purpleProgram.addShader(vertex);
    purpleProgram.addShader(fragment);

    // linkneš program
    purpleProgram.link();


    Shader vertex2(vertex_shader2, "vertex");
    vertex2.Compile();

    Shader fragment2(fragment_shader2, "fragment");
    fragment2.Compile();


    ShaderProgram colorProgram;
    colorProgram.addShader(vertex2);
    colorProgram.addShader(fragment2);

    // linkneš program
    colorProgram.link();

    Rotate r(45.0f, glm::vec3(0,1,0));
    Rotate r1(45.0f, glm::vec3(0,1,0));
    Rotate r2(90.0f, glm::vec3(0,1,0));
    Scale s(0.2f, 0.2f, 0.0f);
    Scale s2(1.0f, 1.0f, 1.0f);

    Tranform tvlevo(-4.0f, 0.0f, 0.0f);
    Tranform tvpravo(4.0f, 0.0f, 0.0f);
    Tranform tnahoru(0.0f, 3.0f, 0.0f);
    Tranform tdolu(0.0f, -3.0f, 0.0f);

//vytvoření první trans
    Transformation t1;
    t1.addTrans(&s);
    t1.addTrans(&r);

    Transformation p;
    p.addTrans(&tvpravo);
    //využiju kompozit (jako parametr dám trans1)
    p.addTrans(&t1);

    Transformation l;
    l.addTrans(&tvlevo);
    l.addTrans(&t1);

    Transformation n;
    n.addTrans(&tnahoru);
    n.addTrans(&t1);

    Transformation d;
    d.addTrans(&tdolu);
    d.addTrans(&t1);

    Transformation t6;
    t6.addTrans(&s);

    Transformation t7;
    t7.addTrans(&s2);

    Transformation t8;
    t8.addTrans(&r1);
    t8.addTrans(&l);

    Transformation t9;
    t9.addTrans(&r2);
    t9.addTrans(&l);

    Rotate rSpin(0.0f, glm::vec3(0,0,1));
    rSpin.setSpeed(glm::radians(1440.0f));

    Transformation tSpin;
    tSpin.addTrans(&rSpin);

    DrawableObject* tr = new DrawableObject(trojuhelnik, purpleProgram, n);
    DrawableObject* suzi1 = new DrawableObject(suzi, colorProgram, t1);
    DrawableObject* stromek1 = new DrawableObject(stromek, colorProgram, p);
    DrawableObject* suzi2 = new DrawableObject(suzi, colorProgram, t8);
    DrawableObject* suzi3 = new DrawableObject(suzi, colorProgram, t9);
    DrawableObject* křok2 = new DrawableObject(křok, purpleProgram, d);
    DrawableObject* křok3 = new DrawableObject(křok, colorProgram, t7);
    DrawableObject* obj2spin = new DrawableObject(trojuhelnik, purpleProgram, tSpin);
    DrawableObject* koule1 = new DrawableObject(koule, colorProgram, p);
    DrawableObject* koule2 = new DrawableObject(koule, colorProgram, l);
    DrawableObject* koule3 = new DrawableObject(koule, colorProgram, n);
    DrawableObject* koule4 = new DrawableObject(koule, colorProgram, d);


    static Scene scene1;
    static Scene scene2;
    static Scene scene3;

    scene1.addObject(tr);
    scene1.addObject(stromek1);
    scene1.addObject(křok2);
    scene1.addObject(křok3);
    scene1.addObject(suzi1);
    scene1.addObject(suzi3);
    scene1.addObject(suzi2);
    scene2.addObject(koule1);
    scene2.addObject(koule2);
    scene2.addObject(koule3);
    scene2.addObject(koule4);
    scene3.addObject(obj2spin);


    manager.addScene(&scene1);
    manager.addScene(&scene2);
    manager.addScene(&scene3);

    app.Run();

    return 0;
}