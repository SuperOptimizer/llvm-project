//===-- include/flang/Parser/parsing.h --------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef FORTRAN_PARSER_PARSING_H_
#define FORTRAN_PARSER_PARSING_H_

#include "instrumented-parser.h"
#include "message.h"
#include "options.h"
#include "parse-tree.h"
#include "provenance.h"
#include "flang/Parser/preprocessor.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>
#include <string>

namespace Fortran::parser {

class Parsing {
public:
  explicit Parsing(AllCookedSources &);
  ~Parsing();

  bool consumedWholeFile() const { return consumedWholeFile_; }
  const char *finalRestingPlace() const { return finalRestingPlace_; }
  AllCookedSources &allCooked() { return allCooked_; }
  const AllCookedSources &allCooked() const { return allCooked_; }
  Messages &messages() { return messages_; }
  std::optional<Program> &parseTree() { return parseTree_; }

  const CookedSource &cooked() const { return DEREF(currentCooked_); }

  const SourceFile *Prescan(const std::string &path, Options);
  void EmitPreprocessedSource(
      llvm::raw_ostream &, bool lineDirectives = true) const;
  void EmitPreprocessorMacros(llvm::raw_ostream &) const;
  void DumpCookedChars(llvm::raw_ostream &) const;
  void DumpProvenance(llvm::raw_ostream &) const;
  void DumpParsingLog(llvm::raw_ostream &) const;
  void Parse(llvm::raw_ostream &debugOutput);
  void ClearLog();

  void EmitMessage(llvm::raw_ostream &o, const char *at,
      const std::string &message, const std::string &prefix,
      llvm::raw_ostream::Colors color = llvm::raw_ostream::SAVEDCOLOR,
      bool echoSourceLine = false) const {
    allCooked_.allSources().EmitMessage(o,
        allCooked_.GetProvenanceRange(CharBlock(at)), message, prefix, color,
        echoSourceLine);
  }

private:
  Options options_;
  AllCookedSources &allCooked_;
  CookedSource *currentCooked_{nullptr};
  Messages messages_;
  bool consumedWholeFile_{false};
  const char *finalRestingPlace_{nullptr};
  std::optional<Program> parseTree_;
  ParsingLog log_;
  Preprocessor preprocessor_{allCooked_.allSources()};
};
} // namespace Fortran::parser
#endif // FORTRAN_PARSER_PARSING_H_
