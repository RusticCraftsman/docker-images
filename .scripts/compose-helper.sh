#!/bin/bash

usage() {
  echo "Usage: $0 <parent_directory>"
  echo "This script recursively finds all 'docker-compose.ymle' files in the given directory,"
  echo "and copies them as 'docker-compose.yml' in their respective directories without overwriting."
  echo "this is useful to convert example files into usable environment ones, i save you the cp commands, you're welcome."
  exit 1
}

PARENT_DIRECTORY=$1

# Check if the parent directory argument is provided.
if [ -z "$PARENT_DIRECTORY" ]; then
  printf "Error: argument was expected \n\n"
  usage
fi

# Check if the provided argument is a valid directory. this is for that guy who is trapped with bash.
if [ ! -d "$PARENT_DIRECTORY" ]; then
  echo "Error: Directory '$PARENT_DIRECTORY' not found."
  exit 1
fi

# Use find to locate all files named 'docker-compose.ymle' within the parent directory.
# The -print0 and IFS= read -r -d '' part handles paths that could contain spaces or newlines, i don't really do this myself but whatever.
find "$PARENT_DIRECTORY" -type f -name "docker-compose.ymle" -print0 | while IFS= read -r -d '' source_file; do


  directory=$(dirname "$source_file")

  destination_file="$directory/docker-compose.yml"

  if [ ! -e "$destination_file" ]; then
    echo "Copied '$source_file' to '$destination_file'"
  else
    echo "Skipped copying to '$destination_file' as it already exists."
  fi

  # -n for not overwriting
  cp -n "$source_file" "$destination_file"

done

printf "\nWe are done here, have fun."
