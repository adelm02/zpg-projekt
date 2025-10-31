#include "Application.h"
#include "SceneManager.h"
#include "Tranform.h"
#include "Transformation.h"

// Global variables for Earth and Moon transformations
Tranform moveEarth(5.0f, 0.0f, 0.0f);
Tranform moveMoon(6.2f, 0.0f, 0.0f);
Transformation tEarth;
Transformation tMoon;

int main() {
    SceneManager manager;
    Application app(manager);
    
    if (!app.init(800, 600, "ZPG")) {
        return -1;
    }
    
    manager.initializeScenes();
    app.Run();
    
    return 0;
}