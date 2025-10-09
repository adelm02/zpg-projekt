//
// Created by Adéla Martynková on 04.10.2025.
//

#ifndef ZPGPROJ_ABSTRACTTRANS_H
#define ZPGPROJ_ABSTRACTTRANS_H
#include <glm/fwd.hpp>


class AbstractTrans {
public:
    virtual glm::mat4 getMatrix() const = 0;
    virtual ~AbstractTrans();
    virtual void apply() = 0;
    virtual void update(float){}
};


#endif //ZPGPROJ_ABSTRACTTRANS_H