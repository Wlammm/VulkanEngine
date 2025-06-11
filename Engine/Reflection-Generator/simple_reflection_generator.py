#!/usr/bin/env python3
"""
C++ Reflection Generator
Generates reflection information for C++ classes in a game engine project.
"""

import os
import re
import argparse
from pathlib import Path
from typing import List, Dict, Tuple, Set

class ReflectionGenerator:
    def __init__(self):
        # Regex patterns for parsing C++ code
        self.class_pattern = re.compile(r'class\s+(\w+)(?:\s*:\s*(?:public|private|protected)\s+[\w:]+)?\s*\{', re.MULTILINE)
        self.struct_pattern = re.compile(r'struct\s+(\w+)(?:\s*:\s*(?:public|private|protected)\s+[\w:]+)?\s*\{', re.MULTILINE)
        
        # Pattern for member variables (handles various C++ types)
        self.member_pattern = re.compile(
            r'^\s*(?:public|private|protected)?\s*'  # Access specifier (optional)
            r'(?:static\s+)?'                        # Static keyword (optional)
            r'(?:const\s+)?'                         # Const keyword (optional)
            r'(?:mutable\s+)?'                       # Mutable keyword (optional)
            r'((?:std::)?(?:vector|array|map|set|unordered_map|unordered_set|shared_ptr|unique_ptr|weak_ptr)?'  # STL containers/smart pointers
            r'(?:<[^>]+>)?'                          # Template parameters
            r'\s*[\w:]+(?:\s*\*)?)'                  # Type name (with optional pointer)
            r'\s+(\w+)'                              # Variable name
            r'(?:\s*=\s*[^;]+)?'                     # Default value (optional)
            r'\s*;',                                 # Semicolon
            re.MULTILINE
        )
        
        # Common C++ built-in types
        self.builtin_types = {
            'int', 'float', 'double', 'bool', 'char', 'short', 'long',
            'unsigned', 'signed', 'size_t', 'uint32_t', 'int32_t',
            'uint64_t', 'int64_t', 'uint16_t', 'int16_t', 'uint8_t', 'int8_t'
        }
        
        # Types to skip (typically non-serializable)
        self.skip_types = {
            'static', 'const', 'mutable', 'volatile'
        }

    def parse_cpp_file(self, filepath: Path) -> Dict[str, List[Tuple[str, str]]]:
        """Parse a C++ file and extract class/struct information."""
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
        except UnicodeDecodeError:
            # Try with different encoding
            with open(filepath, 'r', encoding='latin-1') as f:
                content = f.read()
        
        # Remove comments to avoid false matches
        content = self.remove_comments(content)
        
        classes = {}
        
        # Find all classes and structs
        for pattern in [self.class_pattern, self.struct_pattern]:
            for match in pattern.finditer(content):
                class_name = match.group(1)
                class_start = match.end()
                
                # Find the matching closing brace
                brace_count = 1
                i = class_start
                while i < len(content) and brace_count > 0:
                    if content[i] == '{':
                        brace_count += 1
                    elif content[i] == '}':
                        brace_count -= 1
                    i += 1
                
                if brace_count == 0:
                    class_content = content[class_start:i-1]
                    members = self.extract_members(class_content)
                    if members:  # Only add classes that have members
                        classes[class_name] = members
        
        return classes

    def remove_comments(self, content: str) -> str:
        """Remove C++ comments from the content."""
        # Remove single-line comments
        content = re.sub(r'//.*$', '', content, flags=re.MULTILINE)
        
        # Remove multi-line comments
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        return content

    def extract_members(self, class_content: str) -> List[Tuple[str, str]]:
        """Extract member variables from class content."""
        members = []
        current_access = 'private'  # Default for classes
        
        lines = class_content.split('\n')
        for line in lines:
            # Check for access specifiers
            if re.match(r'^\s*(public|private|protected)\s*:', line):
                current_access = re.match(r'^\s*(public|private|protected)\s*:', line).group(1)
                continue
            
            # Skip if we're not in public section (adjust based on your needs)
            if current_access != 'public':
                continue
            
            # Look for member variables
            member_match = self.member_pattern.match(line)
            if member_match:
                type_name = member_match.group(1).strip()
                var_name = member_match.group(2).strip()
                
                # Clean up type name
                type_name = self.clean_type_name(type_name)
                
                # Skip certain patterns
                if self.should_skip_member(type_name, var_name):
                    continue
                
                members.append((type_name, var_name))
        
        return members

    def clean_type_name(self, type_name: str) -> str:
        """Clean and normalize type names."""
        # Remove extra whitespace
        type_name = ' '.join(type_name.split())
        
        # Handle const
        type_name = type_name.replace('const ', '')
        
        # Handle references and pointers (for reflection, we typically want the base type)
        type_name = type_name.rstrip('&*')
        
        return type_name.strip()

    def should_skip_member(self, type_name: str, var_name: str) -> bool:
        """Determine if a member should be skipped."""
        # Skip function pointers
        if '(' in type_name and ')' in type_name:
            return True
        
        # Skip arrays for now (would need special handling)
        if '[' in var_name and ']' in var_name:
            return True
        
        # Skip members starting with underscore (typically private implementation details)
        if var_name.startswith('_'):
            return True
        
        return False

    def generate_reflection_code(self, classes: Dict[str, List[Tuple[str, str]]]) -> str:
        """Generate the reflection code for all classes."""
        output_lines = []
        
        # Add header comment
        output_lines.append("// Auto-generated reflection information")
        output_lines.append("// Do not modify this file directly")
        output_lines.append("")
        
        for class_name, members in classes.items():
            output_lines.append(f"// Reflection for {class_name}")
            for type_name, var_name in members:
                reflection_line = f"AddField({class_name}, {type_name}, \"{var_name}\", &{class_name}::{var_name});"
                output_lines.append(reflection_line)
            output_lines.append("")
        
        return '\n'.join(output_lines)

    def scan_directory(self, directory: Path, extensions: Set[str]) -> List[Path]:
        """Recursively scan directory for C++ files."""
        cpp_files = []
        
        for root, dirs, files in os.walk(directory):
            # Skip common build directories
            dirs[:] = [d for d in dirs if d not in {'build', 'Build', 'BUILD', '.git', '.vs', 'Debug', 'Release'}]
            
            for file in files:
                if any(file.endswith(ext) for ext in extensions):
                    cpp_files.append(Path(root) / file)
        
        return cpp_files

    def process_project(self, source_dir: Path, output_file: Path, extensions: Set[str] = None):
        """Process an entire project directory."""
        if extensions is None:
            extensions = {'.h', '.hpp', '.hxx', '.cpp', '.cc', '.cxx'}
        
        print(f"Scanning directory: {source_dir}")
        cpp_files = self.scan_directory(source_dir, extensions)
        print(f"Found {len(cpp_files)} C++ files")
        
        all_classes = {}
        
        for filepath in cpp_files:
            print(f"Processing: {filepath}")
            try:
                classes = self.parse_cpp_file(filepath)
                all_classes.update(classes)
            except Exception as e:
                print(f"Error processing {filepath}: {e}")
        
        print(f"Found {len(all_classes)} classes with reflection info")
        
        # Generate reflection code
        reflection_code = self.generate_reflection_code(all_classes)
        
        # Write to output file
        output_file.parent.mkdir(parents=True, exist_ok=True)
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(reflection_code)
        
        print(f"Reflection code written to: {output_file}")
        
        # Print summary
        for class_name, members in all_classes.items():
            print(f"  {class_name}: {len(members)} members")

def main():
    parser = argparse.ArgumentParser(description="Generate C++ reflection information")
    parser.add_argument("source_dir", help="Source directory to scan")
    parser.add_argument("-o", "--output", default="reflection_generated.cpp", 
                       help="Output file (default: reflection_generated.cpp)")
    parser.add_argument("--extensions", nargs='+', 
                       default=['.h', '.hpp', '.hxx'],
                       help="File extensions to process (default: .h .hpp .hxx)")
    
    args = parser.parse_args()
    
    source_dir = Path(args.source_dir)
    output_file = Path(args.output)
    extensions = set(args.extensions)
    
    if not source_dir.exists():
        print(f"Error: Source directory '{source_dir}' does not exist")
        return 1
    
    generator = ReflectionGenerator()
    generator.process_project(source_dir, output_file, extensions)
    
    return 0

if __name__ == "__main__":
    exit(main())