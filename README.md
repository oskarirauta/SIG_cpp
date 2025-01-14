[![License:MIT](https://img.shields.io/badge/License-MIT-blue?style=plastic)](LICENSE)
[![C++ CI build](../../actions/workflows/build.yml/badge.svg)](../../actions/workflows/build.yml)

### SIG_cpp
C++ signal handler
SIG_cpp does not have other depencies.

### <sub>Note</sub>
[signal_cpp](https://github.com/oskarirauta/signal_cpp) was renamed to SIG_cpp.
signal_cpp is archived and will no longer be updated. It exists because some other
projects still are using it. Future development happens now on SIG_cpp repository.

### <sub>Description</sub>
Library can be used to register Linux signals

### <sub>How to use this?</sub>
 - import as a submodule to directory signal
 - include signal/Makefile.inc
 - link agains $(SIGNAL_OBJS)

### <sub>Example</sub>
Minimal sample code is provided
