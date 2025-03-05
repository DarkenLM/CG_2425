#pragma once

#include "engine/glContext.hpp"

class Object {
    public:
        virtual ~Object() = default;

        /**
         * Draws this object into the currently prepared environment.
         */
        virtual void render() = 0;

        /**
         * Steps the internal state forward one tick for this object.
         * Can be used to animate the object, or any other action that must persist a state between ticks
         * and be updated regularly.
         * 
         * @param deltaTime The time between the current frame and the last frame, to normalize the simulation
         * independently of the FPS (kinda, it's a lot more complicated than I can currently understand).
         */
        virtual void update(float deltaTime) {};

        /**
         * Moves this object back to it's origin.
         * 
         * **NOTE:** It does not necessarily reset the object back to the world's origin, but rather
         * relative to the object's referential.
         */
        virtual void resetPosition() {};

        /** 
         * Moves this object to the absolute position represented by the coordinates passed as arguments.
         * 
         * @param x The X position to move to.
         * @param y The Y position to move to.
         * @param z The Z position to move to.
         */
        virtual void setPosition(float x, float y, float z) {};

        /** 
         * Moves this object to the absolute position represented by the vector passed as arguments.
         * 
         * @param pos The position vector to move to.
         */
        virtual void setPosition(Vector3<float> pos) {};

        /** 
         * Moves this object to the position represented by the coordinates passed as arguments, relative to the object's
         * current position. The values for each coordinate are added to the current object's coordinate values.
         * 
         * @param x The X position to move to.
         * @param y The Y position to move to.
         * @param z The Z position to move to.
         */
        virtual void moveTo(float x, float y, float z) {};

        /** 
         * Moves this object to the position represented by the position vector passed as argument, relative to the 
         * object's current position. The values for each coordinate are added to the current object's coordinate values.
         * 
         * @param pos The position vector to move to.
         */
        virtual void moveTo(Vector3<float> pos) {};

        /**
         * Rotates this object back to it's original rotation.
         * 
         * **NOTE:** It does not necessarily reset the object back to it's original rotation angles relative to the world, 
         * but rather relative to the object's referential.
         */
        virtual void resetRotation() {};

        /** 
         * Rotates this object absolutely using the angles passed as arguments.
         * 
         * @param roll The rotation angle along the X axis.
         * @param pitch The rotation angle along the Y axis.
         * @param yaw The rotation angle along the Z axis.
         */
        virtual void setRotation(float roll, float pitch, float yaw) {};

        /** 
         * Rotates this object relative to the current rotation using the angles passed as arguments. 
         * The values for each angle are added to the current object's angle values.
         * 
         * @param roll The rotation angle along the X axis.
         * @param pitch The rotation angle along the Y axis.
         * @param yaw The rotation angle along the Z axis.
         */
        virtual void rotateTo(float roll, float pitch, float yaw) {};

        /** 
         * Rotates this object along the axis characterized by the first three parameters by 
         * the specified angle. The axis is a unit vector, and it's parameters must be normalized.
         * 
         * @param axisX The X parameter of the axis vector.
         * @param axisY The Y parameter of the axis vector.
         * @param axisZ The Z parameter of the axis vector.
         */
        virtual void rotateAlong(float axisX, float axisY, float axisZ, float angle) {};

        /** 
         * Rotates this object along the axis characterized by the first three parameters of the vector by 
         * the fourth parameter of the vector. The first three parameters of the vector represent a unit 
         * vector, and must be normalized.
         * 
         * @param vec The (axisX, axisY, axisZ, angle) vector.
         */
        virtual void rotateAlong(Vector4<float> vec) {};

        /**
         * Scales this object back to it's original scale.
         * 
         * **NOTE:** It does not necessarily reset the object back to it's true original scale, but rather
         * relative to the object's referential.
         */
        virtual void resetScale() {};

        /** 
         * Scales this object to the absolute scale represented by the values passed as arguments.
         * 
         * @param x The scale on the X axis.
         * @param y The scale on the Y axis.
         * @param z The scale on the Z axis.
         */
        virtual void setScale(float x, float y, float z) {};

        /** 
         * Scales this object to the absolute scale represented by the vector passed as argument.
         * 
         * @param scale The vector representing the scale on each axis.
         */
        virtual void setScale(Vector3<float> scale) {};

        /** 
         * Scales this object relative to to the current scale using the values passed as arguments.
         * 
         * @param x The scale on the X axis.
         * @param y The scale on the Y axis.
         * @param z The scale on the Z axis.
         */
        virtual void scaleTo(float x, float y, float z) {};

        /** 
         * Scales this object relative to to the current scale using the vector passed as argument.
         * 
         * @param scale The vector representing the scale on each axis.
         */
        virtual void scaleTo(Vector3<float> scale) {};
};