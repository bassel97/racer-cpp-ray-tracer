#include "renderer-component.h"

#include <entity.h>

void racer::RendererComponent::UpdateBoundingBox()
{
    if (border_shape_ != NULL)
        border_shape_->UpdateBorders(holdingEntity->transform->GetTransformationMatrix());
}