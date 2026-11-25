/*
 * Copyright (c) 2023 - 2026 Chair for Design Automation, TUM
 * Copyright (c) 2025 - 2026 Munich Quantum Software Company GmbH
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License
 */

#include "mqt/Compiler/Programs.h"
#include "mqt/Dialect/QAssert/IR/QAssertDialect.h"

#include "gtest/gtest.h"

#include "mlir/IR/Dialect.h"
#include "mlir/IR/MLIRContext.h"

using namespace mlir;

// The compiler finds the dialect by name only if the registration patch is
// applied. Loading by type would succeed without it.
TEST(QAssertDialectTest, CompilerContextRegistersDialect) {
  auto context = createCompilerContext();
  auto* dialect = context->getOrLoadDialect("qassert");
  ASSERT_NE(dialect, nullptr);
  EXPECT_TRUE(isa<qassert::QAssertDialect>(dialect));
}
