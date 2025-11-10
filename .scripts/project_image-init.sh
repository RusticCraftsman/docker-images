#!/bin/bash

usage() {
  echo "Usage: $0 <dir_name>"
  echo "This script creates a new directory for a docker project"
  echo "and copies opinionated basic docker files so you got an easy start."
  echo "This script is meant to make the dockerization initialization process easier, i save you the copy and pasting a bunch of text, you're welcome."
  exit 1
}

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TEMPLATE_DIR="${SCRIPT_DIR}/.$0"

# Parse arguments
PROJECT_NAME="${1}"
TARGET_PATH="${2:-.}"

# Validate arguments
if [ -z "$PROJECT_NAME" ]; then
    echo -e "${RED}Error: Project name is required${NC}"
    usage
fi

# Validate project name (alphanumeric, hyphens, underscores)
if ! [[ "$PROJECT_NAME" =~ ^[a-zA-Z0-9_-]+$ ]]; then
    echo -e "${RED}Error: Project name must contain only alphanumeric characters, hyphens, and underscores${NC}"
    exit 1
fi

# Check if template directory exists
if [ ! -d "$TEMPLATE_DIR" ]; then
    echo -e "${RED}Error: Template directory not found at $TEMPLATE_DIR${NC}"
    exit 1
fi

# Create full project path
PROJECT_DIR="${TARGET_PATH}/${PROJECT_NAME}"

# Check if project directory already exists
if [ -d "$PROJECT_DIR" ]; then
    echo -e "${RED}Error: Directory $PROJECT_DIR already exists${NC}"
    exit 1
fi

echo -e "${GREEN}Initializing Docker project: $PROJECT_NAME${NC}"
echo "Target path: $PROJECT_DIR"
echo ""

# Create project directory
mkdir -p "$PROJECT_DIR"

# Copy and process template files
echo "Copying template files..."
for file in "$TEMPLATE_DIR"/*; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        echo "  - Processing $filename"
        
        # Read file content and replace placeholders
        # placeholder being {{PROJECT_NAME}}
        sed "s/{{PROJECT_NAME}}/$PROJECT_NAME/g" "$file" > "$PROJECT_DIR/$filename"
    fi
done

echo -e "${GREEN} Project initialized successfully!${NC}"
echo -e "${YELLOW} REMEMBER to have fun"
