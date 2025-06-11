import clang.cindex
import re
import os
from pathlib import Path

extra_vars = {
    "PHYSX_INCLUDE_DIR": "../../PhysX/physx/include/",
    "VULKAN_SDK": os.getenv("VULKAN_SDK", "")
}

includes_file_path = "../Build/engine_includes.txt"

def read_includes(file_path):
    includes = []
    with open(file_path, 'r') as f:
        for line in f:
            line = line.strip()
            if line:
                # Handle environment variables like %{VULKAN_SDK} if desired
                includes.append(line)
    return includes

def resolve_vars(path, extra_vars={}):
    def replacer(match):
        var = match.group(1)
        # Check both extra_vars and environment
        return extra_vars.get(var, os.getenv(var, ""))
    
    return re.sub(r"%\{(.+?)\}", replacer, path)


def collect_includes():
    includes = read_includes("../Build/engine_includes.txt")
    resolved_includes = [resolve_vars(path, extra_vars) for path in includes]
    resolved_includes = [str(Path(path).resolve()) for path in resolved_includes]
    return resolved_includes

def generate_reflection(header_file):

    includes = collect_includes()

    print(includes)

    clang_args = sum([['-I', inc] for inc in includes], [])
    clang_args += ['-x', 'c++', '--std=c++23']
    clang_args += ['-include', str(Path("../Engine/EnginePch.h").resolve())]

    index = clang.cindex.Index.create()
    tu = index.parse(header_file, clang_args)

    for node in tu.cursor.walk_preorder():
        if node.kind == clang.cindex.CursorKind.CLASS_DECL and node.is_definition():
            print(f"Class: {node.spelling}")
            for child in node.get_children():
                if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                    field_name = child.spelling
                    field_type = child.type.spelling
                    print(f"  Field: {field_name} ({field_type})")

generate_reflection("C:/Users/William/Documents/GitHub/VulkanEngine/Engine/Engine/Components/TransformComponent.h")