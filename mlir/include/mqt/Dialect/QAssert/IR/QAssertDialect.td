// Copyright (c) 2023 - 2026 Chair for Design Automation, TUM
// Copyright (c) 2025 - 2026 Munich Quantum Software Company GmbH
// All rights reserved.
//
// SPDX-License-Identifier: MIT
//
// Licensed under the MIT License

#ifndef MLIR_DIALECT_QASSERT_IR_QASSERTDIALECT_TD
#define MLIR_DIALECT_QASSERT_IR_QASSERTDIALECT_TD

include "mlir/IR/DialectBase.td"

def QAssertDialect : Dialect {
  let name = "qassert";

  let summary = "Assertions for quantum programs.";

  let description = [{
    The qassert dialect states assertions in programs that the MQT Core
    compiler processes. It defines no operations yet.
  }];

  let cppNamespace = "::mlir::qassert";
}

#endif // MLIR_DIALECT_QASSERT_IR_QASSERTDIALECT_TD
