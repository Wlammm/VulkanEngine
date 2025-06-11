
# reflection_generator.py
import clang.cindex
import sys
import os

def setup_clang():
    """Setup clang library - may need adjustment based on your system"""
    # Try common locations for libclang
    possible_paths = [
        '/usr/lib/x86_64-linux-gnu/libclang-14.so.1',
        '/usr/lib/llvm-14/lib/libclang.so.1',
        '/usr/local/lib/libclang.dylib',  # macOS
        'C:\\Program Files\\LLVM\\bin\\libclang.dll',  # Windows
    ]
    
    for path in possible_paths:
        if os.path.exists(path):
            clang.cindex.conf.set_library_file(path)
            break
    else:
        print("Warning: Could not find libclang. You may need to install it or set the path manually.")

def generate_reflection_for_class(cursor, class_name=None):
    """Generate reflection code for a C++ class"""
    if class_name is None:
        class_name = cursor.spelling
    
    fields = []
    methods = []
    
    # Collect fields and methods
    for child in cursor.get_children():
        if child.kind == clang.cindex.CursorKind.FIELD_DECL:
            field_name = child.spelling
            field_type = child.type.spelling
            # Skip private fields (basic check)
            if not field_name.startswith('_') and not field_name.startswith('m_'):
                fields.append((field_name, field_type))
        elif child.kind == clang.cindex.CursorKind.CXX_METHOD:
            if child.access_specifier == clang.cindex.AccessSpecifier.PUBLIC:
                methods.append((child.spelling, child.type.spelling))
    
    if not fields and not methods:
        return
    
    # Generate reflection code
    print(f"// Reflection for {class_name}")
    print(f"template<>")
    print(f"struct reflection_info<{class_name}> {{")
    print(f"    static void register_fields() {{")
    
    for field_name, field_type in fields:
        print(f'        add_field("{field_name}", &{class_name}::{field_name});')
    
    print(f"    }}")
    print(f"}};")
    print()

def find_classes_in_file(filename):
    """Parse a C++ file and find all classes/structs"""
    setup_clang()
    
    # Create index and parse the file
    index = clang.cindex.Index.create()
    
    # Common include paths - adjust as needed for your system
    args = [
        '-x', 'c++',
        '-std=c++17',
        '-I/usr/include',
        '-I/usr/include/c++/11',  # Adjust version as needed
        '-I.',  # Current directory
    ]
    
    try:
        translation_unit = index.parse(filename, args=args)
    except Exception as e:
        print(f"Error parsing file {filename}: {e}")
        return
    
    # Check for parsing errors
    for diag in translation_unit.diagnostics:
        if diag.severity >= clang.cindex.Diagnostic.Error:
            print(f"Parse error: {diag}")
            return
    
    # Find all class/struct declarations
    def visit_node(node, depth=0):
        if node.kind in [clang.cindex.CursorKind.CLASS_DECL, 
                        clang.cindex.CursorKind.STRUCT_DECL]:
            # Only process classes/structs defined in the target file
            if node.location.file and node.location.file.name == filename:
                if node.spelling:  # Skip anonymous classes
                    generate_reflection_for_class(node)
        
        # Recursively visit children
        for child in node.get_children():
            visit_node(child, depth + 1)
    
    visit_node(translation_unit.cursor)

def main():
    if len(sys.argv) != 2:
        print("Usage: python reflection_generator.py <header_file.h>")
        sys.exit(1)
    
    header_file = sys.argv[1]
    
    if not os.path.exists(header_file):
        print(f"Error: File '{header_file}' not found.")
        sys.exit(1)
    
    print("// Auto-generated reflection code")
    print("#pragma once")
    print("#include \"reflection_system.h\"  // Your reflection system header")
    print(f"#include \"{header_file}\"")
    print()
    
    find_classes_in_file(header_file)

if __name__ == "__main__":
    main()