#pragma once

class Skeleton
{
public:
    struct Bone
    {
        glm::vec3 myBindPosition;
        glm::quat myBindRotation;
        glm::vec3 myBindScale;

        int myIndexToParent;
    };

    List<Bone> myJoints{};
};

// TODO: To be continued...