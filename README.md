# MQT Core QAssert

An assertion dialect for the MQT Core compiler (`mqt-core compile`). This
repository holds only the QAssert code. It pins
[MQT Core](https://github.com/munich-quantum-toolkit/core) as a git submodule
and carries a small patch that registers QAssert in the compiler.

The repository follows the layout of
[mqt-cc-qoracle](https://github.com/cda-tum/mqt-cc-qoracle).
[Create an MQT Core extension repository](https://github.com/cda-tum/mqt-cc-qoracle/blob/main/docs/creating-an-extension.md)
explains how such a repository works and why.

## Layout

- `external/mqt-core/`: the pinned MQT Core commit.
- `patches/`: changes to MQT Core, applied to the submodule's working tree.
  They only register QAssert in core targets. All QAssert code lives here.
- `mlir/`: QAssert sources and unit tests, in the same layout as MQT Core's
  `mlir/` tree.
- `docs/glossary.md`: terms that QAssert adds to the MQT Core glossary.
- `AGENTS.md`: rules for coding agents in this repository.

## Build

The build needs LLVM/MLIR 23.1. Unlike a standalone MQT Core build, it does not
download the SDK, so pass `MLIR_DIR`.

```bash
git clone --recurse-submodules https://github.com/cda-tum/mqt-cc-qassert.git
cd mqt-cc-qassert
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DMLIR_DIR=<llvm-prefix>/lib/cmake/mlir -DLLVM_DIR=<llvm-prefix>/lib/cmake/llvm
cmake --build build
ctest --test-dir build -L qassert
```

The default target builds the `mqt-core` executable and the QAssert libraries
and tests. Other MQT Core targets build only on request, for example
`cmake --build build --target mqt-core-mlir-unittests-compiler`. `ctest`
without `-L qassert` also lists those core tests and reports them as not built.

The build needs CMake 3.29 or newer, as MQT Core does. If the system CMake is
older, run `uvx cmake` instead. With GCC 13, add `-DENABLE_IPO=OFF`, because
link-time optimization crashes the compiler when it links `mqt-core`.

Configuration applies the patches to `external/mqt-core` and skips those that
are already applied. It fails if a patch does not apply, so the compiler cannot
silently build without QAssert.

## Continuous integration

`.github/workflows/ci.yml` runs on every push to `main` and on every pull
request. It checks out the pinned MQT Core, applies the patches, builds all MQT
Core and QAssert targets, and runs all tests of both. The two jobs match the
Ubuntu jobs of MQT Core's CI: Debug on x86-64 and Release on ARM, both with GCC.

To run the same build locally, use the presets. They build all of MQT Core,
which takes much longer than the build above.

```bash
cmake --preset release
cmake --build --preset release
ctest --preset release
```

## Start developing

The `qassert` dialect exists and is registered in the compiler, but it has no
operations yet. The unit test in `mlir/unittests/Dialect/QAssert/IR/` checks
that the compiler finds the dialect.

- **Operations:** define them in
  `mlir/include/mqt/Dialect/QAssert/IR/QAssertOps.td`, based on the `QAssertOp`
  class. Put custom parsers, printers, and verifiers in
  `mlir/lib/Dialect/QAssert/IR/`. If operations use QC types, add the QC
  dialect to `dependentDialects` in `QAssertDialect.td` and link
  `MLIRQCDialect`.
- **Passes and conversions:** add them in new `Transforms/` or
  `Conversion/` folders, following the matching QOracle folders. To use them
  from `mqt-core compile`, extend the patch: register passes in
  `registerMQTCompilerPasses` (`mlir/lib/Support/Passes.cpp`) and conversions
  in `mlir/tools/compile/Compile.cpp`. Then run them with
  `build/bin/mqt-core compile --run-pipeline --pass-pipeline='builtin.module(<passes>)' <input.mlir>`.
- **Tests:** add GoogleTest targets under `mlir/unittests/` with the `qassert`
  label.

The MQT Core rules for code, tests, and documentation apply here. See
`external/mqt-core/AGENTS.md` and `external/mqt-core/docs/development.md`.

## Update MQT Core

```bash
git -C external/mqt-core checkout -- .
git -C external/mqt-core fetch origin
git -C external/mqt-core checkout <new-commit>
cmake -S . -B build
```

If a patch no longer applies, apply it by hand in `external/mqt-core`, fix the
conflicts, and regenerate it with `git -C external/mqt-core diff`. Keep the
description at the top of the patch file. Then rebuild and fix any QAssert code
that the core API changes broke. Commit the new submodule commit together with
those fixes.

`.gitmodules` sets `ignore = dirty`, so the applied patches do not show up in
`git status`. Use `git -C external/mqt-core status` to see them.
