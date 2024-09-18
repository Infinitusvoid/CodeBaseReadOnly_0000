import subprocess
import os
import sys

class ClangCompiler:
    def __init__(self, compiler='clang', std='c++17', optimize='O2', debug=False, warnings=True):
        """
        Initialize the compiler wrapper with default settings.
        
        Args:
        - compiler: Name of the compiler ('clang' or 'clang++').
        - std: C++ standard to use (e.g., 'c++17', 'c11').
        - optimize: Optimization level ('O0', 'O1', 'O2', 'O3', 'Ofast').
        - debug: If True, include debugging information.
        - warnings: If True, enable all warnings.
        """
        self.compiler = compiler
        self.std = std
        self.optimize = optimize
        self.debug = debug
        self.warnings = warnings

        self.output_file = None
    def compile(self, source_files, output_file='a.out', additional_flags=None):
        """
        Compile and link multiple source files into an executable.
        
        Args:
        - source_files: List of source files to compile.
        - output_file: Name of the output executable.
        - additional_flags: List of any additional flags to pass to the compiler.
        
        Returns:
        - Compilation success or failure (True/False).
        """
        if not isinstance(source_files, list):
            print("Error: source_files must be a list.")
            return False
        
        if additional_flags is None:
            additional_flags = []

        # Base command
        command = [self.compiler]

        # Standard
        command.append(f'-std={self.std}')

        # Optimization level
        command.append(f'-{self.optimize}')

        # Debugging information
        if self.debug:
            command.append('-g')

        # Warnings
        if self.warnings:
            command.append('-Wall')

        # Additional flags
        command += additional_flags

        # Source files
        command += source_files

        # Output file
        command.append(f'-o{output_file}')

        # After compiling it we need to be able to run it save it's name
        self.output_file = output_file
        
        # Run the command
        try:
            print(f"Running command: {' '.join(command)}")
            subprocess.run(command, check=True)
            print(f"Compilation successful: {output_file}")
            return True
        except subprocess.CalledProcessError as e:
            print(f"Compilation failed with error:\n{e}")
            return False
    def run_program(self, args=None):
        """
        Run the compiled program and display its output in the Jupyter notebook.
        
        Args:
        - executable_file: The compiled executable file to run.
        - args: List of arguments to pass to the executable.
        
        Returns:
        - Program's stdout and stderr.
        """
        if(self.output_file == None):
            print("error : not program to run, you should compile the program first")
            return
        
        if args is None:
            args = []

        command = [f'./{self.output_file}'] + args

        try:
            print(f"Running executable: {' '.join(command)}")
            # Run the program and capture output
            result = subprocess.run(command, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            print(f"Program output:\n{result.stdout}")
            return result.stdout, result.stderr
        except subprocess.CalledProcessError as e:
            print(f"Execution failed with error:\n{e.stderr}")
            return "", e.stderr
