#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/dual_quaternion.hpp>

bool FloatCompare(float a1, float a2);
bool FloatCompare(double a1, double a2);

#define PI (3.14159265358979f)
#define EPSLON_ANGLE (0.001f)

using Vector2d = glm::dvec2;
using Vector2f = glm::vec2;
using Vector2i = glm::int2;

using Vector3i = glm::int3;
using Vector3d = glm::dvec3;
using Vector3f = glm::vec3;

using Vector4d = glm::dvec4;
using Vector4i = glm::int4;
using Vector4f = glm::vec4;

using Vector2ui = glm::uvec2;
using Vector3ui = glm::uvec3;
using Vector4ui = glm::uvec4;

using Matrix4f = glm::mat4x4;
using Matrix3f = glm::mat3x3;

using Matrix4d = glm::dmat4x4;
using Matrix3d = glm::dmat3x3;

using Quaternionf = glm::fquat;
using Quaterniond = glm::dquat;

using DualQuaternionf = glm::fdualquat;
using DualQuaterniond = glm::ddualquat;


