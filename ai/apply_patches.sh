#!/bin/bash
set -e

PROJECT_ROOT=$(dirname $(dirname $(realpath $0)))
REAL_SRC="$PROJECT_ROOT/src"
AI_SRC="$PROJECT_ROOT/ai/src"

echo "=== AI Patch Applicator ==="
echo "Real Source: $REAL_SRC"
echo "AI Source:   $AI_SRC"
echo ""

# Check if there are differences
if diff -r -q "$REAL_SRC" "$AI_SRC" >/dev/null; then
    echo "No changes detected between ai/src/ and src/."
    exit 0
fi

echo "The following files have changes:"
diff -r -q "$REAL_SRC" "$AI_SRC" | grep "differ" || true
echo ""

# Show unified diff
echo "=== DIFF PREVIEW ==="
diff -r -u "$REAL_SRC" "$AI_SRC" --color=always || true
echo "===================="
echo ""

read -p "Do you want to apply these changes to the real src/ directory? [y/N] " response
if [[ "$response" =~ ^([yY][eE][sS]|[yY])$ ]]; then
    echo "Applying changes..."
    # Copy files from AI_SRC to REAL_SRC
    cp -r "$AI_SRC/"* "$REAL_SRC/"
    echo "Changes applied successfully!"
else
    echo "Operation cancelled. No changes made."
fi
