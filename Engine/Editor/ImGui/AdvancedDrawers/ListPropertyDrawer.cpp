#include "EditorPch.h"
#include "ListPropertyDrawer.h"

#include "Engine/Reflection/Type.h"

bool ListPropertyDrawer::DrawsType(const Type* inType) const
{
    return inType->IsA<IList>();
}

bool ListPropertyDrawer::Draw(void* inInstance, const Field& inField) const
{
    IList* list = static_cast<IList*>(inInstance);
    
    check(!inField.GetType()->GetTemplateArguments().IsEmpty() && "Something is wrong if the list does not contain any template arguments...");
    const Type* typeInList = inField.GetType()->GetTemplateArguments().First().myType;

    bool wasModified = false;
    if (ImGui::CollapsingHeader(inField.GetType()->GetName().c_str()))
    {
        for (int i = 0; i < list->size(); ++i)
        {
            Field tempField = Field(std::to_string(i), 0, typeInList, false,false);
            void* elementPointer = (char*)list->GetData() + (list->GetSizeofElement() * i);
            bool result = ImGuiPropertyDrawer::DrawProperty(tempField, elementPointer);
            
            if (result)
                wasModified = true;
        }
    }
    return wasModified;
}
