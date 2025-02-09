#extension GL_EXT_nonuniform_qualifier : enable

const float Epsilon = 0.00001;
const float PI = 3.14159265359;

vec4 LinearToGamma(vec4 inLinear)
{
    return pow(inLinear, vec4(1.0/2.2));
}

vec4 GammaToLinear(vec4 inGamma)
{
    return pow(inGamma, vec4(2.2));
}

vec3 LinearToGamma(vec3 inLinear)
{
    return pow(inLinear, vec3(1.0/2.2));
}

vec3 GammaToLinear(vec3 inGamma)
{
    return pow(inGamma, vec3(2.2));
}

float saturate(float inValue)
{
    return clamp(inValue, 0.0, 1.0);
}

vec2 saturate(vec2 inValue)
{
    return clamp(inValue, vec2(0.0, 0.0), vec2(1.0, 1.0));
}

vec3 saturate(vec3 inValue)
{
    return clamp(inValue, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
}

vec4 saturate(vec4 inValue)
{
    return clamp(inValue, vec4(0.0, 0.0, 0.0, 0.0), vec4(1.0, 1.0, 1.0, 1.0));
}

vec4 LinearColorFromColor(int inColor)
{
    float r = float(inColor & 0xFF) / 255.0;
    float g = float((inColor >> 8) & 0xFF) / 255.0;
    float b = float((inColor >> 16) & 0xFF) / 255.0;
    float a = float((inColor >> 24) & 0xFF) / 255.0;

    return vec4(r, g, b, a);
}

// Shader avoid branching article: https://theorangeduck.com/page/avoiding-shader-conditionals
// USAGE: output of these will either be 1 or 0 depending on the condition is true or not. Simply multiply the output with a number to add it conditionally.

// ==
vec4 when_eq(vec4 x, vec4 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
vec4 when_neq(vec4 x, vec4 y) {
  return abs(sign(x - y));
}

// >
vec4 when_gt(vec4 x, vec4 y) {
  return max(sign(x - y), 0.0);
}

// <
vec4 when_lt(vec4 x, vec4 y) {
  return max(sign(y - x), 0.0);
}

// >=
vec4 when_ge(vec4 x, vec4 y) {
  return 1.0 - when_lt(x, y);
}

// <=
vec4 when_le(vec4 x, vec4 y) {
  return 1.0 - when_gt(x, y);
}

// vec3
// ==
vec3 when_eq(vec3 x, vec3 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
vec3 when_neq(vec3 x, vec3 y) {
  return abs(sign(x - y));
}

// >
vec3 when_gt(vec3 x, vec3 y) {
  return max(sign(x - y), 0.0);
}

// <
vec3 when_lt(vec3 x, vec3 y) {
  return max(sign(y - x), 0.0);
}

// >=
vec3 when_ge(vec3 x, vec3 y) {
  return 1.0 - when_lt(x, y);
}

// <=
vec3 when_le(vec3 x, vec3 y) {
  return 1.0 - when_gt(x, y);
}


// vec2
// ==
vec2 when_eq(vec2 x, vec2 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
vec2 when_neq(vec2 x, vec2 y) {
  return abs(sign(x - y));
}

// >
vec2 when_gt(vec2 x, vec2 y) {
  return max(sign(x - y), 0.0);
}

// <
vec2 when_lt(vec2 x, vec2 y) {
  return max(sign(y - x), 0.0);
}

// >=
vec2 when_ge(vec2 x, vec2 y) {
  return 1.0 - when_lt(x, y);
}

// <=
vec2 when_le(vec2 x, vec2 y) {
  return 1.0 - when_gt(x, y);
}

// float
// ==
float when_eq(float x, float y) {
  return 1.0 - abs(sign(x - y));
}

// !=
float when_neq(float x, float y) {
  return abs(sign(x - y));
}

// >
float when_gt(float x, float y) {
  return max(sign(x - y), 0.0);
}

// <
float when_lt(float x, float y) {
  return max(sign(y - x), 0.0);
}

// >=
float when_ge(float x, float y) {
  return 1.0 - when_lt(x, y);
}

// <=
float when_le(float x, float y) {
  return 1.0 - when_gt(x, y);
}