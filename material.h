#ifndef MATERIAL_H
#define MATERIAL_H


class Material
{
public:
    Material();

    const char *type;
    const char *name;
    const char *normalMap;
    const char *baseColor;
    bool emissive;
    bool reflective;
    const char *texture;

};

#endif // MATERIAL_H
