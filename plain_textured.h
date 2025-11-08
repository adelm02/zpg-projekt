#ifndef PLAIN_TEXTURED_H
#define PLAIN_TEXTURED_H

// Format: X, Y, Z, NX, NY, NZ, U, V (8 floats per vertex)
const float plain_textured[] = {
    // Triangle 1
    1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,  // pravý horní
    1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   10.0f,  0.0f,  // pravý dolní
   -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,    0.0f,  0.0f,  // levý dolní

   // Triangle 2
   -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,    0.0f, 10.0f,  // levý horní
    1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   10.0f, 10.0f,  // pravý horní
   -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,    0.0f,  0.0f   // levý dolní
};

#endif