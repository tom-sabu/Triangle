# AI Patch Workspace

This directory `ai/` is a safe workspace for AI-generated code modifications.

## Rules

1.  **AI Edits**: All AI modifications are performed on file copies located in `ai/src/`.
2.  **Safety**: The real source code in `../../src/` is **NEVER** modified directly by the AI.
3.  **Patching**:
    - The AI modifies files in `ai/src/`.
    - You can review changes by comparing `ai/src/` with `../../src/`.
    - To apply changes to the real project, run `./apply_patches.sh`.

## Structure

-   `src/`: Mirrored copies of the real source files.
-   `cmake/`: Reserved for future CMake patches.
-   `patches/`: Reserved for storing patch files.
-   `apply_patches.sh`: Script to diff and apply approved changes.
