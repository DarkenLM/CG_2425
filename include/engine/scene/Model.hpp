#pragma once

#include <unordered_map>

#include "common/geometry/BaseGeometry.hpp"
#include "common/parser.hpp"
#include "common/util/maputil.hpp"
#include "common/util/xmlutil.hpp"
#include "engine/geometry/GeometryCatalog.hpp"
#include "engine/scene/Object.hpp"

using namespace tinyxml2;

class Model : public Object {
   public:
    Model(const char* source);

#pragma region------- Overrides -------
    void setPosition(float x, float y, float z) override;
    void setPosition(Vector3<float> pos) override;
    void moveTo(float x, float y, float z) override;
    void moveTo(Vector3<float> pos) override;
    void setRotation(float axisX, float axisY, float axisZ) override;
    void rotateAlong(float axisX, float axisY, float axisZ, float angle) override;
    void rotateAlong(Vector4<float> vec) override;
    void setScale(Vector3<float> sv) override;
    void scaleTo(Vector3<float> sv) override;
    void render() override;
#pragma endregion-- -- -- -Overrides-- -- -- -

    const char* getSource();

    const char* getTexture();
    Model* setTexture(const char* texture);

    const char* getColor();
    Model* setColor(const char* color);

    static Model* fromXML(XMLElement* xml);
    void load();

    static Map<GLuint, std::string>& getGeometryVBO();
    static Map<GLuint, std::string>& getGeometryIBO();
    static Map<BaseGeometry*, std::string> getGeometryCache();

   private:
    std::string source;
    std::string texture;
    std::string color;
    BaseGeometry* geometry;
    bool _loaded;

    static Map<BaseGeometry*, std::string> _geometryCache;
    static Map<GLuint, std::string> _geometryVBO;
    static Map<GLuint, std::string> _geometryIBO;
    static BaseGeometry* getOrLoadModel(std::string modelName);

   protected:
    // // Transform
    // std::optional<Vector3<float>> translation;
    // std::optional<Vector4<float>> rotation;
    // std::optional<Vector3<float>> scale;
    // std::vector<TransformType> tfStack;

    // void setTfStack(std::vector<TransformType> tfStack);
};
