#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 FragPos;

uniform float u_MassScale;
uniform bool u_IsBlackHole;
uniform float u_ScaledRadius;
uniform mat4 u_MVP;

void main()
{

    float distanceFromCenter = length(aPos.xz);
    float yDisplacement = 0.0f;

    if (u_IsBlackHole){
        float horizonRadius = clamp(u_ScaledRadius, 0.5, 1.0);

        //float effectiveRadius = sqrt(distanceFromCenter*distanceFromCenter+horizonRadius*horizonRadius);

        float well = -u_MassScale/max((distanceFromCenter-horizonRadius),0.05);
        yDisplacement = well;
    }else {
        float localization = exp(-0.65*distanceFromCenter);

        float plummer = -u_MassScale/
            sqrt(distanceFromCenter*distanceFromCenter+u_ScaledRadius*u_ScaledRadius);

        yDisplacement = plummer*localization;
    }

    vec3 displacedPos = vec3(aPos.x,yDisplacement,aPos.z);

    FragPos = displacedPos;

    gl_Position = u_MVP * vec4(FragPos, 1.0);
}