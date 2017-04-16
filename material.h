#ifndef MATERIAL_H
#define MATERIAL_H


class Material
{
public:
    Material();

    String type;
    String normalMap;
    String baseColor;
    bool emissive;
    bool reflective;
    String texture;

};

#endif // MATERIAL_H
