import clang.cindex
import re
import os
from pathlib import Path

def generate_reflection(header_file):
    index = clang.cindex.Index.create()
    tu = index.parse(header_file)

    for node in tu.cursor.walk_preorder():
        if node.kind == clang.cindex.CursorKind.CLASS_DECL:
            print(node.spelling)

generate_reflection("C:/Users/William/Documents/GitHub/VulkanEngine/Engine/Engine/Components/TransformComponent.h")