/* SPDX-License-Identifier: BSD-2-Clause
 *
 * This file is part of pyosmium. (https://osmcode.org/pyosmium/)
 *
 * Copyright (C) 2025 Sarah Hoffmann <lonvia@denofr.de> and others.
 * For a full list of authors see the git log.
 */
#ifndef PYOSMIUM_IO_H
#define PYOSMIUM_IO_H

#include <osmium/thread/pool.hpp>
#include <osmium/io/any_input.hpp>
#include <osmium/io/any_output.hpp>

namespace pyosmium {

class PyReader
{
public:
    explicit PyReader(std::string fname)
    : thread_pool(new osmium::thread::Pool()), reader(fname, *thread_pool) {}

    explicit PyReader(osmium::io::File const &fname)
    : thread_pool(new osmium::thread::Pool()), reader(fname, *thread_pool) {}

    PyReader(std::string fname, osmium::osm_entity_bits::type etype)
    : thread_pool(new osmium::thread::Pool()), reader(fname, etype, *thread_pool) {}

    PyReader(osmium::io::File const &fname, osmium::osm_entity_bits::type etype)
    : thread_pool(new osmium::thread::Pool()), reader(fname, *thread_pool) {}

    PyReader(std::string fname, osmium::thread::Pool &pool)
    : reader(fname, pool) {}

    PyReader(osmium::io::File const &fname, osmium::thread::Pool &pool)
    : reader(fname, pool) {}

    PyReader(std::string fname, osmium::osm_entity_bits::type etype, osmium::thread::Pool &pool)
    : reader(fname, etype, pool) {}

    PyReader(osmium::io::File const &fname, osmium::osm_entity_bits::type etype, osmium::thread::Pool &pool)
    : reader(fname, etype, pool) {}

    osmium::io::Reader const *get() const { return &reader; }
    osmium::io::Reader *get() { return &reader; }

private:
    std::unique_ptr<osmium::thread::Pool> thread_pool;
    osmium::io::Reader reader;
};


class PyWriter
{
public:
    PyWriter(osmium::io::File file, osmium::io::Header const *header,
             bool overwrite, osmium::thread::Pool *pool)
    : thread_pool(pool ? std::unique_ptr<osmium::thread::Pool>()
                       : std::make_unique<osmium::thread::Pool>()),
      writer(std::move(file),
             header ? *header : osmium::io::Header(),
             overwrite ? osmium::io::overwrite::allow : osmium::io::overwrite::no,
             *(pool ? pool : thread_pool.get()))
    {}

    osmium::io::Writer const *get() const { return &writer; }
    osmium::io::Writer *get() { return &writer; }

private:
    std::unique_ptr<osmium::thread::Pool> thread_pool;
    osmium::io::Writer writer;
};

}

#endif // PYOSMIUM_IO_H
