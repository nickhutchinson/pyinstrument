#!/bin/bash
set -ueo pipefail

SOURCE=$1
DEST=$2
BCP=${BCP:-bcp}

libs=(
    atomic
    container/static_vector.hpp
    exception/all.hpp
    format.hpp
    predef.h
    iterator_adaptors.hpp
    range.hpp
    python.hpp
    thread.hpp
    variant.hpp
    optional.hpp
    utility.hpp
    utility/string_ref.hpp
)

"$BCP" --boost="$SOURCE" "${libs[@]}" "$DEST"

find -E "$DEST" -type d -iregex '.*(examples?|tests?|docs?|mpi)' | xargs rm -fr
find "$DEST" -type f -name shared_ptr_helper.cpp -delete

