
float4 LinearColorFromColor(int inColor)
{
    float r = float(inColor & 0xFF) / 255.0;
    float g = float((inColor >> 8) & 0xFF) / 255.0;
    float b = float((inColor >> 16) & 0xFF) / 255.0;
    float a = float((inColor >> 24) & 0xFF) / 255.0;

    return float4(r, g, b, a);
}