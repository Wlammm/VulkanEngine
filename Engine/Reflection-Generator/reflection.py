import clang.cindex
import re
import os
from pathlib import Path

extra_vars = {
    "PHYSX_INCLUDE_DIR": "../../PhysX/physx/include/",
    "VULKAN_SDK": os.getenv("VULKAN_SDK", "")
}

includes_file_path = "../Build/engine_includes.txt"
headers_root = Path("../Engine/")

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

def build_clang_args(include_paths):
    args = []
    args = sum([['-I', inc] for inc in include_paths], [])
    args += ['-x', 'c++', '--std=c++23']
    args += ['-include', str(Path("../Engine/EnginePch.h").resolve())]
    return args

def find_all_headers():
    headers = []

    for path in headers_root.rglob("*.h"):
        headers.append(path)

    for path in headers_root.rglob("*.hpp"):
        headers.append(path)

    return headers

def parse_file(header_path, clang_args):
    index = clang.cindex.Index.create()
    tu = index.parse(header_path, clang_args)

    for node in tu.cursor.walk_preorder():
        if node.kind == clang.cindex.CursorKind.CLASS_DECL and node.is_definition():
            print(f"Class: {node.spelling}")
            for child in node.get_children():
                if child.kind == clang.cindex.CursorKind.FIELD_DECL:
                    field_name = child.spelling
                    field_type = child.type.spelling
                    print(f"  Field: {field_name} ({field_type})")

def build_unity_file(headers, output_file):
    with open(output_file, "w") as f:
        f.write('include "EnginePch.h"\n')
        for header in headers:
            f.write(f'#include "{header}"\n')

def generate_reflection():

    includes = collect_includes()
    clang_args = build_clang_args(includes)

    headers = find_all_headers()

    build_unity_file(headers, "ReflectionUnityFile.cpp")
    parse_file("ReflectionUnityFile.cpp", clang_args)
    
generate_reflection()