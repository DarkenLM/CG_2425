#include "engine/scene/Model.hpp"

using namespace tinyxml2;

Model::Model(const char* source) {
    this->source = source;
    this->texture = std::string("");
    this->color = std::string("");
    this->geometry = nullptr;
    this->_loaded = false;
}

#pragma region ------- Overrides -------
void Model::render() {
    std::vector<Point3D> vertices = this->geometry->getVertices();

    glBegin(GL_TRIANGLES);
        for (Point3D vertex : vertices) {
            glVertex3f(vertex.getX(), vertex.getY(), vertex.getZ());
        }
    glEnd();
}
#pragma endregion ------- Overrides -------

const char* Model::getTexture() {
    return this->texture.c_str();
}

Model* Model::setTexture(const char *texture) {
    this->texture = texture;
    return this;
}

const char* Model::getColor() {
    return this->color.c_str();
}

Model* Model::setColor(const char *color) {
    this->color = color;
    return this;
}

Model* Model::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "file", file, const char*);
    GET_XML_ELEMENT_ATTRIB(xml, "texture", texture, const char*);
    GET_XML_ELEMENT_ATTRIB(xml, "color", color, const char*);

    Model* obj = new Model(file);
    if (texture) obj->setTexture(texture);
    if (color) obj->setTexture(color);

    return obj;
}

void Model::load() {
    this->geometry = this->getOrLoadModel(this->source);
    this->_loaded = true;
}

Map<BaseGeometry*, std::string> Model::_geometryCache;
BaseGeometry* Model::getOrLoadModel(std::string modelName) {
    auto model = _geometryCache.get(modelName);
    if (model.has_value()) return model.value();

    // BaseGeometry* geometry = new BaseGeometry();
    BaseGeometry *geometry = nullptr;
    if (modelName.size() > 3 && modelName.substr(modelName.size() - 3) == ".3d") {
        geometry = Parser3D::load3DFile(modelName);
    } else if (modelName.size() > 4 && modelName.substr(modelName.size() - 4) == ".obj") {
        geometry = Parser3D::loadObjFile(modelName);
    } else {
        throw std::runtime_error("Unsupported file format: " + modelName);
    }

    if (geometry == nullptr) yeet std::string("Unable to load model.");

    _geometryCache.add(modelName, geometry);

    return geometry;
}