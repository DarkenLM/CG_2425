#define ILUT_USE_OPENGL

#include "engine/scene/Model.hpp"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <unistd.h>  // For getcwd on Linux/macOS

#include <sstream>

using namespace tinyxml2;

extern struct scenestate STATE;  ///< External reference to the global scene state

Model::Model(const char* source, const char* id) {
    this->source = source;
    this->showNormals = false;
    this->texture = std::string("");
    this->color = std::string("");
    this->geometry = nullptr;
    this->_loaded = false;

    if (id == NULL)
        this->id = std::string("");
    else
        this->id = id;
}

#pragma region------- Overrides -------
void Model::setPosition(float x, float y, float z) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(x, y, z);
    } else {
        this->translation = ObjectTranslation(x, y, z);
    }
};

void Model::setPosition(Vector3<float> pos) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    } else {
        this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    }
};

void Model::moveTo(float x, float y, float z) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation.value() += Vector3<float>(x, y, z);
    } else {
        this->translation = ObjectTranslation(x, y, z);
    }
};

void Model::moveTo(Vector3<float> pos) {
    if (this->translation.has_value()) {
        if (!this->translation.value().isDynamic()) this->translation.value() += pos;
    } else {
        this->translation = ObjectTranslation(pos.first, pos.second, pos.third);
    }
};

void Model::setRotation(float axisX, float axisY, float axisZ) {
    this->rotation = ObjectRotation(0.0f, axisX, axisY, axisZ);
}

void Model::setTfStack(std::vector<TransformType> tfstack) {
    this->tfStack = tfstack;
}

std::vector<TransformType> Model::getTrasnformations() const {
    return tfStack;
}

std::optional<ObjectTranslation> Model::getTranslation() const {
    return this->translation;
};

std::optional<ObjectRotation> Model::getRotation() const {
    return this->rotation;
};

std::optional<Vector3<float>> Model::getScale() const {
    return this->scale;
};

std::optional<ObjectMaterial> Model::getMaterial() const {
    return this->material;
};

void Model::rotateAlong(float axisX, float axisY, float axisZ, float angle) {
    if (this->rotation.has_value()) {
        if (!this->rotation.value().isDynamic()) this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
    } else {
        this->rotation = ObjectRotation(angle, axisX, axisY, axisZ);
    }
}

void Model::rotateAlong(Vector4<float> vec) {
    if (this->rotation.has_value()) {
        if (!this->rotation.value().isDynamic()) {
            this->rotation = ObjectRotation(this->rotation.value().getAngle(), vec.first, vec.second, vec.third);
        }
    } else {
        this->rotation = ObjectRotation(0.0f, vec.first, vec.second, vec.third);
    }
}

void Model::setScale(Vector3<float> sv) {
    this->scale = sv;
}

void Model::scaleTo(Vector3<float> sv) {
    if (this->scale.has_value()) {
        this->scale.value() += sv;
    } else {
        this->scale = sv;
    }
}

void Model::processNormals() {
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);  // Bright red normals for visibility

    auto geometry = this->_geometryCache.get(this->source).value();
    const std::vector<Point3D>& vertices = geometry->getVertices();
    const std::vector<Vector3<float>>& normals = geometry->getNormals();

    float normalLength = 0.5f;  // Adjust for visual clarity

    for (size_t i = 0; i < vertices.size(); ++i) {
        const Point3D& v = vertices[i];
        const Vector3<float>& n = normals[i];

        glVertex3f(v.getX(), v.getY(), v.getZ());  // start point
        glVertex3f(v.getX() + n.first * normalLength,
                   v.getY() + n.second * normalLength,
                   v.getZ() + n.third * normalLength);  // end point
    }
    glColor3f(1.0f, 1.0f, 1.0f);  // Bright red normals for visibility
    glEnd();
    glEnable(GL_LIGHTING);
}

void Model::loadingMaterial() {
    // Use default material values (gray tones) if material doesn't exist
    auto matOpt = this->getMaterial();
    // Vector3<float> ambientColor(50, 50, 50);
    Vector3<float> ambientColor(50, 0, 0);
    Vector3<float> diffuseColor(200, 200, 200);
    Vector3<float> specularColor(0, 0, 0);
    Vector3<float> emissiveColor(0, 0, 0);
    float shininess = 0.0f;

    if (matOpt.has_value()) {
        auto& mat = matOpt.value();
        ambientColor = mat.getAmbient();
        diffuseColor = mat.getDiffuse();
        specularColor = mat.getSpecular();
        emissiveColor = mat.getEmissive();
        shininess = mat.getShininess();
    }

    float ambient[] = {
        ambientColor.first / 255.0f,
        ambientColor.second / 255.0f,
        ambientColor.third / 255.0f,
        1.0f};

    float diffuse[] = {
        diffuseColor.first / 255.0f,
        diffuseColor.second / 255.0f,
        diffuseColor.third / 255.0f,
        1.0f};

    float specular[] = {
        specularColor.first / 255.0f,
        specularColor.second / 255.0f,
        specularColor.third / 255.0f,
        1.0f};

    float emissive[] = {
        emissiveColor.first / 255.0f,
        emissiveColor.second / 255.0f,
        emissiveColor.third / 255.0f,
        1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void Model::render() {
    glPushMatrix();
    for (auto tt : this->tfStack) {
        switch (tt) {
            case TRANSFORM_TRANSLATE: {
                if (this->translation.has_value()) {
                    float pos[3], deriv[3];
                    float rotationMatrix[4][4];
                    float xAxis[3], yAxis[3], zAxis[3];

                    float elapsedTime = (float)glutGet(GLUT_ELAPSED_TIME) / (1000.0f * this->translation.value().getTime());
                    this->translation.value().getInterpolatedPosition(pos, deriv, elapsedTime);
                    this->translation.value().getCurrentRotation(xAxis, yAxis, zAxis, deriv, &rotationMatrix[0][0]);

                    // TODO: Implement the ui for this
                    if (true) {
                        this->translation.value().getCurve().renderCatmullRomCurve();
                    }
                    // TODO: Implement the ui for this
                    if (true) {
                        this->translation.value().drawMyCordSystem(pos, xAxis, yAxis, zAxis);
                    }

                    glTranslatef(pos[0], pos[1], pos[2]);

                    if (this->translation.value().getAlign()) {
                        glMultMatrixf(&rotationMatrix[0][0]);
                    }
                }
                break;
            }
            case TRANSFORM_ROTATE: {
                if (this->rotation.has_value()) {
                    if (this->rotation.value().isDynamic()) {
                    } else {
                        Vector4<float> vec = this->rotation.value().getVector4();
                        glRotatef(vec.first, vec.second, vec.third, vec.fourth);
                    }
                }
                break;
            }
            case TRANSFORM_SCALE: {
                _TRANSFORM_IF3(glScalef, this->scale);
                break;
            }
        }
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    if (this->textureId != 0) {
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, this->textureId);
        // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);  // Ignora luz e mostra só textura

        glBindBuffer(GL_ARRAY_BUFFER, this->_geometryTBO.get(this->source).value());
        glTexCoordPointer(2, GL_FLOAT, 0, 0);
    }

    this->loadingMaterial();

    if (this->showNormals) {
        this->processNormals();
    }

    glBindBuffer(GL_ARRAY_BUFFER, this->_geometryVBO.get(this->source).value());
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, this->_geometryNBO.get(this->source).value());
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_geometryIBO.get(this->source).value());

    size_t indexCount = this->_geometryCache.get(this->source).value()->getIndices().size();

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (this->textureId != 0) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

#pragma endregion-- -- -- -Overrides-- -- -- -

const char* Model::getSource() const {
    return this->source.c_str();
}

const bool Model::getShowNormals() const {
    return this->showNormals;
};

bool* Model::getShowNormalsPtr() {
    return &showNormals;
}

const bool Model::setShowNormals() {
    return this->showNormals;
};

const char* Model::getTexture() {
    return this->texture.c_str();
}

Model* Model::setTexture(const char* texture) {
    this->texture = texture;
    return this;
}

const char* Model::getColor() {
    return this->color.c_str();
}

Model* Model::setColor(const char* color) {
    this->color = color;
    return this;
}

std::string Model::getId() {
    return this->id;
}

unsigned int Model::getTextureId() const {
    return this->textureId;
}

void Model::setTextureId(unsigned int id) {
    this->textureId = id;
}

GLuint loadTexture(Model* obj) {
    ILuint imageID;
    ilGenImages(1, &imageID);
    ilBindImage(imageID);

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);  // Must come after ilEnable

    std::string fullPath = "Textures/" + std::string(obj->getTexture());
    std::cout << "Loading texture: " << fullPath << std::endl;

    if (!ilLoadImage((ILstring)fullPath.c_str()) || ilGetError() != IL_NO_ERROR) {
        std::stringstream err;
        err << "Image loading failed: " << ilGetError() << std::endl;
        ilDeleteImages(1, &imageID);
        yeet err.str();
    }

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    GLuint texID = ilutGLBindTexImage();
    if (ilGetError() != IL_NO_ERROR) {
        std::stringstream err;
        err << "Texture binding failed: " << ilGetError() << std::endl;
        ilDeleteImages(1, &imageID);
        yeet err.str();
    }

    ilDeleteImages(1, &imageID);
    obj->textureId = texID;
    return texID;
}

Model* Model::fromXML(XMLElement* xml) {
    GET_XML_ELEMENT_ATTRIB_OR_FAIL(xml, "file", file, const char*);
    GET_XML_ELEMENT(xml, "texture", textureElem);
    GET_XML_ELEMENT_ATTRIB(xml, "color", color, const char*);

    // Load transforms, if they exist
    // std::optional<Vector3<float>> translate = std::nullopt;
    std::optional<ObjectTranslation> translate = std::nullopt;
    std::optional<ObjectRotation> rotate = std::nullopt;
    std::optional<Vector3<float>> scale = std::nullopt;
    std::vector<TransformType> tfStack;

    std::optional<ObjectMaterial> material = std::nullopt;

    GET_XML_ELEMENT(xml, "transform", _transform);
    if (_transform != NULL) {
        XMLElement* _tfProp = _transform->FirstChildElement();
        do {
            std::string name = std::string(_tfProp->Name());
            if (name == "translate") {
                ObjectTranslation _trans;

                GET_XML_ELEMENT_ATTRIB(_tfProp, "time", ttime, int)
                if (ttime != 0) {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "align", talign, float);

                    std::vector<Point3D> points;
                    GET_XML_ELEMENT(_transform, "point", _point);
                    do {
                        if (!_point) break;  // Element does not exist in XML.
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "x", px, float);
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "y", py, float);
                        GET_XML_ELEMENT_ATTRIB_OR_FAIL(_point, "z", pz, float);

                        Point3D point = Point3D(px, py, pz);
                        points.push_back(point);
                    } while ((_point = _point->NextSiblingElement("point")));

                    _trans = ObjectTranslation(ttime, talign, points);
                } else {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", tx, float);
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ty, float);
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", tz, float);

                    _trans = ObjectTranslation(tx, ty, tz);
                }

                translate = _trans;
                tfStack.push_back(TRANSFORM_TRANSLATE);
            } else if (name == "rotate") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", rx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", ry, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", rz, float);

                GET_XML_ELEMENT_ATTRIB(_tfProp, "angle", rangle, float);
                if (rangle != 0) {
                    rotate = ObjectRotation(rangle, rx, ry, rz);
                } else {
                    GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "time", rtime, int);
                    rotate = ObjectRotation(rtime, rx, ry, rz);
                }
                tfStack.push_back(TRANSFORM_ROTATE);
            } else if (name == "scale") {
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "x", sx, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "y", sy, float);
                GET_XML_ELEMENT_ATTRIB_OR_FAIL(_tfProp, "z", sz, float);

                scale = Vector3(sx, sy, sz);
                tfStack.push_back(TRANSFORM_SCALE);
            }
        } while ((_tfProp = _tfProp->NextSiblingElement()));
    }

    GET_XML_ELEMENT(xml, "color", _color);
    if (_color != NULL) {
        Vector3<float> diffuse, ambient, specular, emissive;
        float shininess;

        {
            GET_XML_ELEMENT_OR_FAIL(_color, "diffuse", _diffuse)
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_diffuse, "R", _r, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_diffuse, "G", _g, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_diffuse, "B", _b, float);
            diffuse = Vector3(_r, _g, _b);
        }
        {
            GET_XML_ELEMENT_OR_FAIL(_color, "ambient", _ambient)
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_ambient, "R", _r, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_ambient, "G", _g, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_ambient, "B", _b, float);
            ambient = Vector3(_r, _g, _b);
        }
        {
            GET_XML_ELEMENT_OR_FAIL(_color, "specular", _specular)
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_specular, "R", _r, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_specular, "G", _g, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_specular, "B", _b, float);
            specular = Vector3(_r, _g, _b);
        }
        {
            GET_XML_ELEMENT_OR_FAIL(_color, "emissive", _emissive)
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_emissive, "R", _r, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_emissive, "G", _g, float);
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_emissive, "B", _b, float);
            emissive = Vector3(_r, _g, _b);
        }
        {
            GET_XML_ELEMENT_OR_FAIL(_color, "shininess", _shininess)
            GET_XML_ELEMENT_ATTRIB_OR_FAIL(_shininess, "value", _svalue, float);
            shininess = _svalue;
        }

        material = ObjectMaterial(diffuse, ambient, specular, emissive, shininess);
    }

    GET_XML_ELEMENT_ATTRIB(xml, "id", id, const char*);

    Model* obj = new Model(file, id);
    if (textureElem != nullptr) {
        GET_XML_ELEMENT_ATTRIB_OR_FAIL(textureElem, "file", texFile, const char*);
        obj->setTexture(texFile);
        // fuckAround {
        //     loadTexture(obj);
        // } findOut(std::string e) {
        //     yeet std::string("Unable to load texture: ") + e;
        // }
    }

    if (color) obj->setColor(color);
    if (translate.has_value()) obj->translation = translate;
    if (rotate.has_value()) obj->rotation = rotate;
    if (scale.has_value()) obj->setScale(scale.value());
    obj->setTfStack(tfStack);

    if (material.has_value()) obj->material = material;

    return obj;
}

void Model::load() {
    this->geometry = this->getOrLoadModel(this->source);
    this->_loaded = true;

    if (!this->texture.empty()) {
        fuckAround {
            loadTexture(this);
        }
        findOut(std::string e) {
            yeet std::string("Unable to load texture: ") + e;
        }
    }
}

Map<BaseGeometry*, std::string> Model::getGeometryCache() {
    return _geometryCache;
}

Map<GLuint, std::string>& Model::getGeometryVBO() {
    return _geometryVBO;
}

Map<GLuint, std::string>& Model::getGeometryTBO() {
    return _geometryTBO;
}

Map<GLuint, std::string>& Model::getGeometryNBO() {
    return _geometryNBO;
};

Map<GLuint, std::string>& Model::getGeometryIBO() {
    return _geometryIBO;
}

Map<BaseGeometry*, std::string> Model::_geometryCache;

Map<GLuint, std::string> Model::_geometryVBO;

Map<GLuint, std::string> Model::_geometryTBO;

Map<GLuint, std::string> Model::_geometryNBO;

Map<GLuint, std::string> Model::_geometryIBO;

BaseGeometry* Model::getOrLoadModel(std::string modelName) {
    auto model = _geometryCache.get(modelName);
    if (model.has_value()) return model.value();

    // BaseGeometry* geometry = new BaseGeometry();
    BaseGeometry* geometry = nullptr;
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