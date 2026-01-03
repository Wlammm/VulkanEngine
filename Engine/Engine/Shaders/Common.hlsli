static const float Epsilon = 0.00001;
static const float PI = 3.14159265359;

float4 LinearToGamma(float4 inLinear)
{
    return pow(inLinear, (1.0f/2.2f).xxxx);
}

float4 GammaToLinear(float4 inGamma)
{
    return pow(inGamma, (2.2f).xxxx);
}

float3 LinearToGamma(float3 inLinear)
{
    return pow(inLinear, (1.0/2.2).xxx);
}

float3 GammaToLinear(float3 inGamma)
{
    return pow(inGamma, (2.2).xxx);
}

float saturate(float inValue)
{
    return clamp(inValue, 0.0, 1.0);
}

float2 saturate(float2 inValue)
{
    return clamp(inValue, float2(0.0, 0.0), float2(1.0, 1.0));
}

float3 saturate(float3 inValue)
{
    return clamp(inValue, float3(0.0, 0.0, 0.0), float3(1.0, 1.0, 1.0));
}

float4 saturate(float4 inValue)
{
    return clamp(inValue, float4(0.0, 0.0, 0.0, 0.0), float4(1.0, 1.0, 1.0, 1.0));
}

float4 LinearColorFromColor(int inColor)
{
    float r = float(inColor & 0xFF) / 255.0;
    float g = float((inColor >> 8) & 0xFF) / 255.0;
    float b = float((inColor >> 16) & 0xFF) / 255.0;
    float a = float((inColor >> 24) & 0xFF) / 255.0;

    return float4(r, g, b, a);
}

// Shader avoid branching article: https://theorangeduck.com/page/avoiding-shader-conditionals
// USAGE: output of these will either be 1 or 0 depending on the condition is true or not. Simply multiply the output with a number to add it conditionally.

// ==
float4 when_eq(float4 x, float4 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
float4 when_neq(float4 x, float4 y) {
  return abs(sign(x - y));
}

// >
float4 when_gt(float4 x, float4 y) {
  return max(sign(x - y), 0.0);
}

// <
float4 when_lt(float4 x, float4 y) {
  return max(sign(y - x), 0.0);
}

// >=
float4 when_ge(float4 x, float4 y) {
  return 1.0 - when_lt(x, y);
}

// <=
float4 when_le(float4 x, float4 y) {
  return 1.0 - when_gt(x, y);
}

// float3
// ==
float3 when_eq(float3 x, float3 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
float3 when_neq(float3 x, float3 y) {
  return abs(sign(x - y));
}

// >
float3 when_gt(float3 x, float3 y) {
  return max(sign(x - y), 0.0);
}

// <
float3 when_lt(float3 x, float3 y) {
  return max(sign(y - x), 0.0);
}

// >=
float3 when_ge(float3 x, float3 y) {
  return 1.0 - when_lt(x, y);
}

// <=
float3 when_le(float3 x, float3 y) {
  return 1.0 - when_gt(x, y);
}


// float2
// ==
float2 when_eq(float2 x, float2 y) {
  return 1.0 - abs(sign(x - y));
}

// !=
float2 when_neq(float2 x, float2 y) {
  return abs(sign(x - y));
}

// >
float2 when_gt(float2 x, float2 y) {
  return max(sign(x - y), 0.0);
}

// <
float2 when_lt(float2 x, float2 y) {
  return max(sign(y - x), 0.0);
}

// >=
float2 when_ge(float2 x, float2 y) {
  return 1.0 - when_lt(x, y);
}

// <=
float2 when_le(float2 x, float2 y) {
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