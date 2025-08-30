#!/bin/bash
# bootstrap_cpp_project.sh
# Usage: ./bootstrap_cpp_project.sh MyProject

if [ -z "$1" ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

echo "Cloning Google Test..."
git clone --depth=1 https://github.com/google/googletest.git third_party/googletest

echo "Bootstrapped C++ project '$PROJECT_NAME' with Google Test."
echo "Run './$PROJECT_NAME/build.sh' to compile the project."
