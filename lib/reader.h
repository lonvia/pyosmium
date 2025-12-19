/* SPDX-License-Identifier: BSD-2-Clause
 *
 * This file is part of pyosmium. (https://osmcode.org/pyosmium/)
 *
 * Copyright (C) 2025 Sarah Hoffmann <lonvia@denofr.de> and others.
 * For a full list of authors see the git log.
 */
#ifndef PYOSMIUM_READER_H
#define PYOSMIUM_READER_H

#include <osmium/thread/pool.hpp>
#include <osmium/io/any_input.hpp>

namespace pyosmium {

class ReaderWithPool
{
public:
    ReaderWithPool(std::string fname)
    : thread_pool(), reader(fname, thread_pool) {}

    ReaderWithPool(osmium::io::File const &fname)
    : thread_pool(), reader(fname, thread_pool) {}

    ReaderWithPool(std::string fname, osmium::osm_entity_bits::type etype)
    : thread_pool(), reader(fname, etype, thread_pool) {}

    ReaderWithPool(osmium::io::File const &fname, osmium::osm_entity_bits::type etype)
    : thread_pool(), reader(fname, thread_pool) {}

    osmium::thread::Pool thread_pool;
    osmium::io::Reader reader;
};

}

#endif // PYOSMIUM_READER_H

