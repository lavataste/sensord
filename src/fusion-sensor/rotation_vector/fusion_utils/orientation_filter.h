/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// released in android-11.0.0_r9

#ifndef ANDROID_FUSION_H
#define ANDROID_FUSION_H

#include "quat.h"
#include "mat.h"
#include "vec.h"
#include "errors.h"

namespace android {

typedef mat<float, 3, 4> mat34_t;

enum FUSION_MODE{
    FUSION_9AXIS, // use accel gyro mag
    FUSION_NOMAG, // use accel gyro (game rotation, gravity)
    FUSION_NOGYRO, // use accel mag (geomag rotation)
    NUM_FUSION_MODE
};

class orientation_filter {
    /*
     * the state vector is made of two sub-vector containing respectively:
     * - modified Rodrigues parameters
     * - the estimated gyro bias
     */
    quat_t  x0;
    vec3_t  x1;

    /*
     * the predicated covariance matrix is made of 4 3x3 sub-matrices and it is
     * semi-definite positive.
     *
     * P = | P00  P10 | = | P00  P10 |
     *     | P01  P11 |   | P10t P11 |
     *
     * Since P01 = transpose(P10), the code below never calculates or
     * stores P01.
     */
    mat<mat33_t, 2, 2> P;

    /*
     * the process noise covariance matrix
     */
    mat<mat33_t, 2, 2> GQGt;

public:
    orientation_filter();
    void init(int mode = FUSION_9AXIS);
    void handleGyro(const vec3_t w, float dT);
    status_t handleAcc(const vec3_t a, float dT);
    status_t handleMag(const vec3_t m);
    vec4_t getAttitude() const;
    vec3_t getBias() const;
    mat33_t getRotationMatrix() const;
    bool hasEstimate() const;

private:
    struct Parameter {
        float gyroVar;
        float gyroBiasVar;
        float accStdev;
        float magStdev;
    } mParam;

    mat<mat33_t, 2, 2> Phi;
    vec3_t Ba, Bm;
    uint32_t mInitState;
    float mGyroRate;
    vec<vec3_t, 3> mData;
    size_t mCount[3];
    int mMode;

    enum { ACC=0x1, MAG=0x2, GYRO=0x4 };
    bool checkInitComplete(int, const vec3_t& w, float d = 0);
    void initFusion(const vec4_t& q0, float dT);
    void checkState();
    void predict(const vec3_t& w, float dT);
    void update(const vec3_t& z, const vec3_t& Bi, float sigma);
    static mat34_t getF(const vec4_t& p);
    static vec3_t getOrthogonal(const vec3_t &v);
};

}

#endif // ANDROID_FUSION_H
