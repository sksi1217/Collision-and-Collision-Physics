// System/CollisionChecks.h
#pragma once

#include "../Components/Colliders/ColliderCircleComponent.h"
#include "../Components/Colliders/ColliderBoxComponent.h"

namespace CollisionChecks {
    bool CheckCircleCircle(
        ColliderCircleComponent* a,
        ColliderCircleComponent* b);

    bool CheckBoxBox(
        ColliderBoxComponent* a,
        ColliderBoxComponent* b);

    bool CheckCircleBox(
        ColliderCircleComponent* circle,
        ColliderBoxComponent* box);
}