#include "Base.h"
#include "Material.h"
#include "Scene.h"
#include "Node.h"

namespace gameplay
{

Material::Material(void) : _light(NULL)
{
    _effect = new Effect();
}

Material::~Material(void)
{
    delete _effect;
}

unsigned int Material::getTypeId(void) const
{
    return MATERIAL_ID;
}
const char* Material::getElementName(void) const
{
    return "Material";
}

void Material::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    //write(_parameters, file);
    //write(_effect, file);
}
void Material::writeText(FILE* file)
{
    fprintf(file, "material %s\n", this->materialId.c_str());
    fprintf(file, "{\n");
    fprintf(file, "\ttechnique\n");
    fprintf(file, "\t{\n");
    fprintf(file, "\t\tpass\n");
    fprintf(file, "\t\t{\n");
    fprintf(file, "\t\t\tu_worldViewProjectionMatrix = WORLD_VIEW_PROJECTION_MATRIX\n");
    if(_light != NULL)
    {
        fprintf(file, "\t\t\tu_inverseTransposeWorldViewMatrix = INVERSE_TRANSPOSE_WORLD_VIEW_MATRIX\n");
        fprintf(file, "\t\t\tu_worldViewMatrix = WORLD_VIEW_MATRIX\n");
        fprintf(file, "\t\t\tu_cameraPosition = CAMERA_WORLD_POSITION\n");
        
//        switch (_light->getLightType()) {
//            case Light::PointLight:
//                fprintf(file, "\t\t\tu_worldViewMatrix = WORLD_VIEW_MATRIX\n");
//                fprintf(file, "\t\t\tu_cameraPosition = CAMERA_VIEW_POSITION\n");
//                break;
//            case Light::SpotLight:
//                fprintf(file, "\t\t\tu_worldViewMatrix = WORLD_VIEW_MATRIX\n");
//                fprintf(file, "\t\t\tu_cameraPosition = CAMERA_WORLD_POSITION\n");
//                break;
//            case Light::DirectionalLight:
//            case Light::AmbientLight:
//            default:
//                fprintf(file, "\t\t\tu_worldViewMatrix = WORLD_VIEW_MATRIX\n");
//                fprintf(file, "\t\t\tu_cameraPosition = CAMERA_WORLD_POSITION\n");
//                break;
//        }
        
    }
    _effect->writeEffect(file, _light);
    fprintf(file, "\t\t}\n");
    fprintf(file, "\t}\n");
    fprintf(file, "}\n\n");
}

Effect& Material::getEffect() const
{
    return *_effect;
}
void Material::setMaterialId(std::string materialId)
{
    this->materialId = materialId;
}
std::string Material::getMaterialId()
{
    return this->materialId;
}
void Material::setLight(Light* light)
{
    _light = light;
}
Light* Material::getLight()
{
    return _light;
}
}
