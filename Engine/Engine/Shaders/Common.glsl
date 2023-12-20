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