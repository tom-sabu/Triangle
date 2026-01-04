#!/usr/bin/env bash
set -e

echo "🔍 Showing AI changes vs real src/:"
git diff --no-index src ai/src || true

echo
read -p "Apply these AI patches to real src/? (y/N): " yn
if [[ "$yn" != "y" && "$yn" != "Y" ]]; then
    echo "❌ Cancelled."
    exit 0
fi

echo "📦 Applying patches..."
rsync -av --delete ai/src/ src/

git status --short

read -p "Commit these changes to git? (y/N): " yn
if [[ "$yn" == "y" || "$yn" == "Y" ]]; then
    git add src
    git commit -m "AI patch: applied changes"
    git push
    echo "✅ AI patch committed."
else
    echo "⚠ Changes applied but NOT committed."
fi

