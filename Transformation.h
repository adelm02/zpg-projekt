#ifndef ZPGPROJ_TRANSFORMATION_H
#define ZPGPROJ_TRANSFORMATION_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AbstractTrans.h"

class Transformation : public AbstractTrans{
private:
    std::vector<AbstractTrans*> transformations;
public:
    Transformation() = default;
    void addTrans(AbstractTrans* trans);
    glm::mat4 getMatrix() const override;
    void apply() override;
    void update(float dt) override;
};


#endif //ZPGPROJ_TRANSFORMATION_H