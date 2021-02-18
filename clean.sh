#!/bin/bash

find filters/                                                  \
    -mindepth 1                                                \
  ! -name '*.c' -and                                           \
  ! -name '*.h' -and                                           \
  ! -name 'Makefile.am'                                        \
    -delete                                                    \
    -print
