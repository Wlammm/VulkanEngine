import ctypes
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
        headers.append(path.resolve())

    for path in headers_root.rglob("*.hpp"):
        headers.append(path.resolve())

    return headers

# Load libclang (you may need to adjust the path)
libclang = ctypes.cdll.LoadLibrary(clang.cindex.Config().get_filename())

# Define argument/return types for clang_getSpecializedCursorTemplate
libclang.clang_getSpecializedCursorTemplate.argtypes = [clang.cindex.Cursor]
libclang.clang_getSpecializedCursorTemplate.restype = clang.cindex.Cursor

def is_specialization(cursor):
    specialized = libclang.clang_getSpecializedCursorTemplate(cursor)
    return specialized is not None and specialized.kind != clang.cindex.CursorKind.INVALID_FILE

def get_fully_qualified_name(node):
    names = []
    while node is not None and node.kind != clang.cindex.CursorKind.TRANSLATION_UNIT:

        if node.kind in (
            clang.cindex.CursorKind.NAMESPACE,
            clang.cindex.CursorKind.CLASS_DECL,
            clang.cindex.CursorKind.STRUCT_DECL
        ):
            if node.spelling:  # Only add if name exists (ignore anonymous)
                names.insert(0, node.spelling)
        if node.kind is clang.cindex.CursorKind.CLASS_TEMPLATE:
            return ""
        node = node.semantic_parent
    return "::".join(names)

def get_fully_qualified_type_name(node):
    original_node = node
    type_name = node.type.spelling

    # Special edge case for template types. They just come back as FIELD_DECL or CLASS_DECL so no other way of knowing that I know of.
    if '<' in type_name or '>' in type_name:
        return type_name

    node = node.type.get_declaration()

    # cindex struggles with finding declarations for nested classes. In those cases we'll need to just hope the type spelling works.
    if node.kind == clang.cindex.CursorKind.NO_DECL_FOUND:
        return type_name

    names = []
    while node is not None and node.kind != clang.cindex.CursorKind.TRANSLATION_UNIT:
        print(f"{node.kind} : Spelling: {node.spelling}")
        if node.kind is clang.cindex.CursorKind.NO_DECL_FOUND:
            return type_name
        
        if node.kind in (
            clang.cindex.CursorKind.CLASS_DECL, 
            clang.cindex.CursorKind.STRUCT_DECL, 
            clang.cindex.CursorKind.NAMESPACE, 
            clang.cindex.CursorKind.TYPE_ALIAS_DECL, 
            clang.cindex.CursorKind.TYPEDEF_DECL, 
            clang.cindex.CursorKind.ENUM_DECL,
            clang.cindex.CursorKind.CLASS_TEMPLATE):
            names.insert(0, node.spelling)

        node = node.semantic_parent
    
    type_name = "::".join(names)
    return type_name

def process_class(node, reflection_database, all_header_files):
    class_name = get_fully_qualified_name(node)
    if class_name.startswith("(unnamed") or class_name.startswith("(anonymous") or class_name == "":
        return

    file_path = Path(node.location.file.name).resolve()
    if file_path not in all_header_files:
        return

    print(f"Class: {class_name}")
    reflection_database[class_name] = []

    for child in node.get_children():
        if child.kind == clang.cindex.CursorKind.FIELD_DECL:
            field_name = child.spelling
            
            field_type = get_fully_qualified_type_name(child)

            reflection_database[class_name].append((field_name, field_type))
        elif (node.kind == clang.cindex.CursorKind.CLASS_DECL or node.kind == clang.cindex.CursorKind.STRUCT_DECL) and node.is_definition():
            # Recursively process nested class
            print(f"Found nested class {child.spelling}")
            process_class(child, reflection_database, all_header_files)

def generate_reflection_database(unity_file_path, clang_args, all_header_files):
    index = clang.cindex.Index.create()
    tu = index.parse(unity_file_path, clang_args)

    reflection_database = {}

    for node in tu.cursor.walk_preorder():
        if (node.kind == clang.cindex.CursorKind.CLASS_DECL or node.kind == clang.cindex.CursorKind.STRUCT_DECL) and node.is_definition():
            if is_specialization(node):
                continue

            process_class(node, reflection_database, all_header_files)

    return reflection_database

def extract_includes_from_unity_file(unity_file):
    with open(unity_file, "r") as f:
        lines = [line.strip() for line in f.readlines()[1:] if line.strip()]
    return lines

def generate_reflection_file_from_database(output_file, reflection_database, unity_file):
    with open(output_file, "w") as f:
        f.write(
            '#pragma once\n' \
            '#include "Engine/Utils/StdIncludes.hpp"\n' \
            '#include "Engine/Reflection/Class.h"\n' \
            '#include "Engine/Reflection/ReflectionSystem.h"\n' \
            '\n\n' \
            '#define private public\n' \
            '#define protected public\n' \
            '\n' \
            '// BEGIN INCLUDES FOR REFLECTED TYPES\n')
        
        # include all reflected classes here. The unity file
        includes = extract_includes_from_unity_file(unity_file)
        for include in includes:
            f.write(f'{include}\n')

        f.write('// END INCLUDES FOR REFLECTED TYPES\n\n')

        f.write('class GeneratedReflectionData\n' \
            '{\n' \
            'public:\n' \
            '\tstatic void RegisterReflectionData(Engine& inEngine)\n' \
            '\t{\n' \
            '\t\tReflectionSystem& reflectionSystem = inEngine.GetReflectionSystem(); \n' \
            '\n\n' \
            '\t\t// Create all classes.\n' \
            '\t\t{\n')
        
        for class_name in reflection_database:
            f.write(f'\t\t\treflectionSystem.AddClass("{class_name}", typeid({class_name}).name());\n')

        f.write(
            '\t\t}\n' \
            '\n\n' \
            '\t\t// Add all fields & add base classes.\n' \
            '\t\t{\n')
        
        for class_name in reflection_database:
            f.write('\t\t\t{\n')
            f.write(f'\t\t\t\tClass* currentClass = reflectionSystem.GetMutableClass<{class_name}>();\n')
            fields = reflection_database[class_name]
            for field_name, field_type in fields:
                f.write(f'\t\t\t\tcurrentClass->AddField(Field("{field_name}", offsetof({class_name}, {field_name}), reflectionSystem.GetClass<{field_type}>(), false, false /* THESE 2 ARE TO BE IMPLEMENTED LATER*/));\n')
            f.write('\t\t\t\t// Base classes are not implemented yet.\n')
            f.write('\t\t\t}\n')
        
        f.write('\t\t}\n' \
        '\t}\n' \
        '};\n' \
        '#undef private\n' \
        '#undef protected\n')


def build_unity_file(headers, output_file):
    with open(output_file, "w") as f:
        f.write('#include "EnginePch.h"\n')
        for header in headers:
            f.write(f'#include "{header}"\n')

def generate_reflection():

    includes = collect_includes()
    clang_args = build_clang_args(includes)

    headers = find_all_headers()

    build_unity_file(headers, "ReflectionUnityFile.cpp")
    print(headers)
    reflection_database = generate_reflection_database("ReflectionUnityFile.cpp", clang_args, headers)
    generate_reflection_file_from_database( "../Launcher/GeneratedReflectionData.hpp", reflection_database, "ReflectionUnityFile.cpp")

generate_reflection()