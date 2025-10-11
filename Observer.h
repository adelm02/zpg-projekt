//
// Created by Adéla Martynková on 09.10.2025.
//

#ifndef ZPGPROJ_OBSERVER_H
#define ZPGPROJ_OBSERVER_H
#include <glm/fwd.hpp>


class Observer {
    public:
    virtual void update(glm::mat4 view, glm::vec3 viewPos, glm::mat4 projection) = 0;
};


#endif //ZPGPROJ_OBSERVER_H